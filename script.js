// ThingSpeak URL (Fetching only the very last object directly)
const THINGSPEAK_URL = "https://api.thingspeak.com/channels/3307953/feeds/last.json?api_key=B9LJ6LGL7FZ5B8U3";

// DOM Elements
const tempEl = document.getElementById("temp");
const humEl = document.getElementById("hum");
const soilEl = document.getElementById("soil");
const rainEl = document.getElementById("rain");
const cropEl = document.getElementById("crop");

/**
 * Triggers a subtle CSS scale/glow animation to indicate changing data
 */
function animateValue(element, newValue) {
    // Only animate if the data actually changed
    if (element.innerText !== String(newValue)) {
        element.innerText = newValue;

        // Reset animation
        element.classList.remove('animate-update');
        void element.offsetWidth; // Trigger reflow
        element.classList.add('animate-update');
    }
}

/**
 * Fetch and Parse ThingSpeak IoT Data
 */
async function fetchIoTData() {
    try {
        const response = await fetch(THINGSPEAK_URL);

        if (!response.ok) {
            throw new Error(`HTTP Error: ${response.status}`);
        }

        const data = await response.json();

        // ----------------------------------------
        // DATA MAPPING (ESP8266 -> ThingSpeak -> Web)
        // field1 → Temperature
        // field2 → Humidity
        // field3 → Soil
        // field4 → Rain (0/1)
        // field5 → Crop
        // ----------------------------------------

        // Handle numeric fields safely
        const temp = data.field1 && data.field1 !== "null" ? parseFloat(data.field1).toFixed(1) : "--";
        const hum = data.field2 && data.field2 !== "null" ? parseFloat(data.field2).toFixed(1) : "--";
        const soil = data.field3 && data.field3 !== "null" ? parseFloat(data.field3).toFixed(1) : "--";

        // Handle Rain (0 = No Rain, 1 = Raining)
        let rainStatus = "--";
        if (data.field4 !== undefined && data.field4 !== null && data.field4 !== "null") {
            const r = parseInt(data.field4, 10);
            if (r === 0) rainStatus = "No";
            else if (r === 1) rainStatus = "Yes";
            else rainStatus = data.field4; // Fallback
        }

        // Handle Crop Prediction
        const crop = data.field5 && data.field5 !== "null" ? data.field5 : "Awaiting Data... ⌛";

        // Update UI with Animations
        animateValue(tempEl, temp);
        animateValue(humEl, hum);
        animateValue(soilEl, soil);
        animateValue(rainEl, rainStatus);

        // Remove "Loading..." immediately on first fetch
        animateValue(cropEl, crop);

    } catch (error) {
        console.error("Failed to fetch data from ThingSpeak:", error);
        if (cropEl.innerText === "Loading..." || cropEl.innerText === "Awaiting Data... ⌛") {
            cropEl.innerText = "Connection Error ❌";
            cropEl.style.color = "#ef4444"; // Red for error
        }
    }
}

// 1. Initial Data Fetch (Instantly on Page Load)
fetchIoTData();

// 2. Continuous Polling (Every 5 Seconds)
setInterval(fetchIoTData, 5000);
