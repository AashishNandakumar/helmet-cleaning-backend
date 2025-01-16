const mongoose = require("mongoose");

const paymentSchema = new mongoose.Schema({
    // userId: {
    //     type: mongoose.Schema.Types.ObjectId,
    //     ref: "User",
    //     required: true,
    // },
    razorpayOrderId: {
        type: String,
        required: true,
    },
    status: {
        type: String,
        required: true,
        enum: ["created", "paid", "failed"],
    },
    amount: {
        type: Number,
        required: true,
    },
    currency: {
        type: String,
        required: true,
    },
    createdAt: {
        type: Date,
        default: Date.now,
    },
});

module.exports = mongoose.model("Payment", paymentSchema);
