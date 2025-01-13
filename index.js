const express = require("express");
const dotenv = require("dotenv");

dotenv.config();

const app = express();
const PORT = process.env.PORT || 3000;

app.get("/health", (req, res) => {
    res.status(200).json({ message: "Server is running!" });
});

app.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
});
