const awsIot = require("aws-iot-device-sdk");

const device = awsIot.device({
    keyPath: "certs/private.pem.key",
    certPath: "certs/certificate.pem.crt",
    caPath: "certs/root-CA.pem",
    clientId: "helmet-cleaning-backend",
    host: "a2vvufbiwbsbvo-ats.iot.ap-south-1.amazonaws.com",
});

device.on("connect", () => {
    console.log("connected to AWS Iot Core!");
});

device.on("error", (error) => {
    console.error("error connecting to AWS IoT Core:", error);
});

module.exports = device;
