#ifndef PORTAL_MATERIAL_H
#define PORTAL_MATERIAL_H

#include <Arduino.h>
#include "web_material.h"

// WiFi Configuration Portal HTML - Built using MaterialPage helper class
// Demonstrates reusable Material Design framework with automatic dark mode
String getPortalHTML() {
    String html = MaterialPage::getHeader("WiFi Setup - Smart Light");

    // App bar
    html += MaterialPage::getAppBar("Smart Home Light", "WiFi Configuration");

    // Main configuration card
    html += MaterialPage::startCard();

    // Form
    html += "<form id=\"wifiForm\">";

    // WiFi Network dropdown
    html += MaterialPage::formField("WiFi Network",
        "<select id=\"ssid\" name=\"ssid\" required>"
        "<option value=\"\">Scanning networks...</option>"
        "</select>");

    // Password field
    html += MaterialPage::formField("Password",
        "<input type=\"password\" id=\"password\" name=\"password\" autocomplete=\"off\" required>");

    // Submit button
    html += MaterialPage::button("Connect to Network", "submit", "submitBtn");

    // Status message
    html += MaterialPage::statusMessage("status");

    html += "</form>";

    html += MaterialPage::endCard();

    // JavaScript for WiFi scanning and form submission
    html += R"html(
<script>
// Scan for WiFi networks on page load
fetch('/scan')
    .then(r => r.json())
    .then(networks => {
        const select = document.getElementById('ssid');
        select.innerHTML = '<option value="">Select a network...</option>';
        networks.forEach(n => {
            const option = document.createElement('option');
            option.value = n.ssid;
            option.textContent = `${n.ssid} (${n.rssi} dBm)`;
            select.appendChild(option);
        });
    })
    .catch(e => {
        const select = document.getElementById('ssid');
        select.innerHTML = '<option value="">Network scan failed</option>';
    });

// Handle form submission
document.getElementById('wifiForm').addEventListener('submit', async (e) => {
    e.preventDefault();

    const ssid = document.getElementById('ssid').value;
    const password = document.getElementById('password').value;
    const submitBtn = document.getElementById('submitBtn');
    const status = document.getElementById('status');

    if (!ssid) {
        status.className = 'status error';
        status.textContent = '✗ Please select a network';
        return;
    }

    submitBtn.disabled = true;
    submitBtn.innerHTML = '<span class="spinner"></span>Connecting...';
    status.className = 'status';
    status.style.display = 'none';

    try {
        const response = await fetch('/connect', {
            method: 'POST',
            headers: {'Content-Type': 'application/x-www-form-urlencoded'},
            body: `ssid=${encodeURIComponent(ssid)}&password=${encodeURIComponent(password)}`
        });

        const result = await response.json();

        if (result.success) {
            status.className = 'status success';
            status.textContent = '✓ Connected! Device will restart...';
            setTimeout(() => window.location.reload(), 3000);
        } else {
            status.className = 'status error';
            status.textContent = `✗ ${result.message || 'Connection failed'}`;
            submitBtn.disabled = false;
            submitBtn.textContent = 'Connect to Network';
        }
    } catch (error) {
        status.className = 'status error';
        status.textContent = `✗ Request failed: ${error.message}`;
        submitBtn.disabled = false;
        submitBtn.textContent = 'Connect to Network';
    }
});
</script>
)html";

    html += MaterialPage::getFooter();

    return html;
}

#endif
