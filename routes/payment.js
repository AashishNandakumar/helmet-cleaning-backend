const express = require("express");
const Razorpay = require("razorpay");
const router = express.Router();
const Payment = require("../models/Payment");
const device = require("../iot/awsIot");

const { RAZORPAY_KEY_ID, RAZORPAY_KEY_SECRET } = process.env;
const razorpay = new Razorpay({
    key_id: RAZORPAY_KEY_ID,
    key_secret: RAZORPAY_KEY_SECRET,
});

router.post("/create-order", async (req, res) => {
    const { userId, amount, currency, receipt } = req.body;

    try {
        const order = await razorpay.orders.create({
            amount: amount * 100,
            currency,
            receipt,
        });

        const payment = new Payment({
            userId,
            razorpayOrderId: order.id,
            status: "created",
            amount,
            currency,
        });
        await payment.save();

        res.status(200).json({ success: true, order });
    } catch (error) {
        console.error("Error creating order:", error);
        res.status(500).json({ success: false, error: error.message });
    }
});

// Webhook route for Razorpay
router.post("/payment-webhook", async (req, res) => {
    const secret = RAZORPAY_KEY_SECRET;
    const signature = req.headers["x-razorpay-signature"];
    const body = JSON.stringify(req.body);

    const crypto = require("crypto");
    const generatedSignature = crypto
        .createHmac("sha256", secret)
        .update(body)
        .digest("hex");

    if (generatedSignature === signature) {
        console.log("Payment verified:", req.body);

        const { order_id } = req.body.payload.payment.entity;
        const status = req.body.event.split(".")[1];
        console.log(`status: ${status}`);
        console.log(`orderid: ${order_id}`);

        await Payment.findOneAndUpdate(
            { razorpayOrderId: order_id },
            { status },
            { new: true },
        );

        // IMP: only in test mode (in live - paid status will be captured)
        if (status === "paid") {
            // create MQTT message
            const message = JSON.stringify({
                event: "payment_success",
                orderId: order_id,
                timestamp: new Date().toISOString(),
            });

            // publish message to AWS IoT Core topic
            device.publish("helmet/cleaning/success", message, (error) => {
                if (error) {
                    console.error("failed to publish message:", error);
                    res.status(500).json({
                        success: false,
                        error: error.message,
                    });
                } else {
                    console.log("message published", message);
                    res.status(200).json({ success: true });
                }
            });
        } else {
            res.status(200).json({ success: true });
        }
    } else {
        console.log("Invalid Signature:", req.body);
        res.status(400).json({ success: false });
    }
});

router.post("/publish", (req, res) => {
    device.publish(
        "helmet/cleaning/success",
        JSON.stringify(req.body),
        (error) => {
            if (error) {
                console.error("failed to publish message:", error);
                res.status(500).json({
                    success: false,
                    error: error.message,
                });
            } else {
                console.log("message published", JSON.stringify(req.body));
                res.status(200).json({ success: true });
            }
        },
    );
});

module.exports = router;
