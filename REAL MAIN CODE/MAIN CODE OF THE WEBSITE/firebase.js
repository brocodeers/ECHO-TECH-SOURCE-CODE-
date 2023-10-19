const firebaseConfig = {
    apiKey: "AIzaSyD9X96aRDLGD_52bcds9pcNC5ybzGtzfys",
    authDomain: "echo-tech.firebaseapp.com",
    databaseURL: "https://echo-tech-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "echo-tech",
    storageBucket: "echo-tech.appspot.com",
    messagingSenderId: "920714883326",
    appId: "1:920714883326:web:b54f58eb091ad43d3c35af",
    measurementId: "G-X1B5FFSPDD"
};


firebase.initializeApp(firebaseConfig);

var firebaseRef = firebase.database().ref("sensorData");

function updateData(snapshot) {
    const data = snapshot.val();
    document.getElementById("temperature").textContent = data.temperature || "N/A";
    document.getElementById("humidity").textContent = data.humidity || "N/A";
    document.getElementById("co2").textContent = data.CO2 || "N/A";
    document.getElementById("light").textContent = data.light || "N/A";
    document.getElementById("moisture").textContent = data.Moisture || "N/A";
    document.getElementById("humimessage").textContent = data.humimessage || "N/A";
    document.getElementById("tempmessage").textContent = data.tempmessage || "N/A";

    document.getElementById("message_CO2").textContent = data.message_CO2 || "N/A";

    document.getElementById("msg_moist").textContent = data.msg_moist || "N/A";

    document.getElementById("message_light").textContent = data.message_light || "N/A";
}
firebaseRef.on("value", updateData);
