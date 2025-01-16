const dotenv = require("dotenv");
dotenv.config();

const express = require("express");
const mongoose = require("mongoose");
const cors = require("cors");
const bodyParser = require("body-parser");
const paymentRoutes = require("./routes/payment");

const connectDB = async () => {
    try {
        await mongoose.connect(process.env.MONGO_URI);
        console.log("MongoDB connected successfully");
    } catch (error) {
        console.error("MongoDB connection failed: ", error.message);
        process.exit(1);
    }
};
connectDB();

const app = express();
const PORT = process.env.PORT || 3000;

app.use(cors());
app.use(bodyParser.json());
app.use("/api/payment", paymentRoutes);

app.get("/health", (req, res) => {
    res.status(200).json({ message: "Server is running!" });
});

app.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
});
