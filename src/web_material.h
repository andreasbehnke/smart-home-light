#ifndef WEB_MATERIAL_H
#define WEB_MATERIAL_H

#include <Arduino.h>

// Material Design CSS - Reusable for all web pages
const char MATERIAL_CSS[] PROGMEM = R"css(
/* Material Design Variables */
:root {
    --md-primary: #1976d2;
    --md-primary-dark: #1565c0;
    --md-primary-light: #42a5f5;
    --md-accent: #ff4081;
    --md-background: #fafafa;
    --md-surface: #ffffff;
    --md-error: #f44336;
    --md-success: #4caf50;
    --md-text-primary: rgba(0, 0, 0, 0.87);
    --md-text-secondary: rgba(0, 0, 0, 0.60);
    --md-text-disabled: rgba(0, 0, 0, 0.38);
    --md-divider: rgba(0, 0, 0, 0.12);
    --md-elevation-1: 0 2px 1px -1px rgba(0,0,0,0.2), 0 1px 1px 0 rgba(0,0,0,0.14), 0 1px 3px 0 rgba(0,0,0,0.12);
    --md-elevation-2: 0 3px 1px -2px rgba(0,0,0,0.2), 0 2px 2px 0 rgba(0,0,0,0.14), 0 1px 5px 0 rgba(0,0,0,0.12);
    --md-elevation-4: 0 2px 4px -1px rgba(0,0,0,0.2), 0 4px 5px 0 rgba(0,0,0,0.14), 0 1px 10px 0 rgba(0,0,0,0.12);
}

/* Dark Mode */
@media (prefers-color-scheme: dark) {
    :root {
        --md-primary: #90caf9;
        --md-primary-dark: #42a5f5;
        --md-primary-light: #bbdefb;
        --md-background: #121212;
        --md-surface: #1e1e1e;
        --md-text-primary: rgba(255, 255, 255, 0.87);
        --md-text-secondary: rgba(255, 255, 255, 0.60);
        --md-text-disabled: rgba(255, 255, 255, 0.38);
        --md-divider: rgba(255, 255, 255, 0.12);
    }
}

* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
}

body {
    font-family: 'Roboto', 'Segoe UI', system-ui, -apple-system, sans-serif;
    background: var(--md-background);
    color: var(--md-text-primary);
    min-height: 100vh;
    display: flex;
    flex-direction: column;
    padding: 16px;
    -webkit-font-smoothing: antialiased;
}

/* App Bar */
.app-bar {
    background: var(--md-primary);
    color: white;
    padding: 16px;
    margin: -16px -16px 24px -16px;
    box-shadow: var(--md-elevation-4);
}

.app-bar h1 {
    font-size: 20px;
    font-weight: 500;
    letter-spacing: 0.15px;
}

.app-bar .subtitle {
    font-size: 14px;
    opacity: 0.7;
    margin-top: 4px;
}

/* Card */
.card {
    background: var(--md-surface);
    border-radius: 4px;
    box-shadow: var(--md-elevation-2);
    padding: 24px;
    max-width: 500px;
    width: 100%;
    margin: 0 auto 16px auto;
}

.card-title {
    font-size: 20px;
    font-weight: 500;
    margin-bottom: 16px;
    color: var(--md-text-primary);
}

/* Form Elements */
.form-field {
    position: relative;
    margin-bottom: 24px;
}

label {
    display: block;
    font-size: 12px;
    font-weight: 500;
    letter-spacing: 0.4px;
    color: var(--md-text-secondary);
    margin-bottom: 8px;
    text-transform: uppercase;
}

input[type="text"],
input[type="password"],
input[type="number"],
input[type="email"],
select {
    width: 100%;
    height: 56px;
    padding: 16px;
    font-size: 16px;
    font-family: inherit;
    background: var(--md-surface);
    border: 1px solid var(--md-divider);
    border-radius: 4px;
    color: var(--md-text-primary);
    transition: all 0.2s;
    -webkit-appearance: none;
    appearance: none;
}

select {
    background-image: url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='24' height='24' viewBox='0 0 24 24' fill='%23666'%3E%3Cpath d='M7 10l5 5 5-5z'/%3E%3C/svg%3E");
    background-repeat: no-repeat;
    background-position: right 8px center;
    padding-right: 40px;
}

@media (prefers-color-scheme: dark) {
    select {
        background-image: url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='24' height='24' viewBox='0 0 24 24' fill='%23aaa'%3E%3Cpath d='M7 10l5 5 5-5z'/%3E%3C/svg%3E");
    }
}

input:hover,
select:hover {
    border-color: var(--md-text-primary);
}

input:focus,
select:focus {
    outline: none;
    border-color: var(--md-primary);
    border-width: 2px;
    padding: 15px;
}

/* Button - Material Raised */
button,
.btn {
    width: 100%;
    height: 48px;
    padding: 0 24px;
    font-size: 14px;
    font-weight: 500;
    font-family: inherit;
    letter-spacing: 1.25px;
    text-transform: uppercase;
    background: var(--md-primary);
    color: white;
    border: none;
    border-radius: 4px;
    box-shadow: var(--md-elevation-2);
    cursor: pointer;
    transition: all 0.2s;
    -webkit-tap-highlight-color: transparent;
    text-decoration: none;
    display: inline-flex;
    align-items: center;
    justify-content: center;
}

button:hover,
.btn:hover {
    background: var(--md-primary-dark);
    box-shadow: var(--md-elevation-4);
}

button:active,
.btn:active {
    box-shadow: var(--md-elevation-1);
}

button:disabled,
.btn:disabled {
    background: var(--md-text-disabled);
    color: var(--md-text-secondary);
    box-shadow: none;
    cursor: not-allowed;
}

.btn-secondary {
    background: transparent;
    color: var(--md-primary);
    box-shadow: none;
    border: 1px solid var(--md-divider);
}

.btn-secondary:hover {
    background: rgba(25, 118, 210, 0.08);
    box-shadow: none;
}

/* Status Messages */
.status {
    margin-top: 16px;
    padding: 16px;
    border-radius: 4px;
    font-size: 14px;
    display: none;
    animation: slideIn 0.2s;
}

.status.success {
    background: rgba(76, 175, 80, 0.1);
    color: var(--md-success);
    border-left: 4px solid var(--md-success);
    display: block;
}

.status.error {
    background: rgba(244, 67, 54, 0.1);
    color: var(--md-error);
    border-left: 4px solid var(--md-error);
    display: block;
}

.status.info {
    background: rgba(25, 118, 210, 0.1);
    color: var(--md-primary);
    border-left: 4px solid var(--md-primary);
    display: block;
}

@keyframes slideIn {
    from {
        opacity: 0;
        transform: translateY(-8px);
    }
    to {
        opacity: 1;
        transform: translateY(0);
    }
}

/* Loading State */
.loading {
    text-align: center;
    color: var(--md-text-secondary);
    padding: 16px;
}

/* Material Spinner */
.spinner {
    display: inline-block;
    width: 20px;
    height: 20px;
    border: 2px solid var(--md-text-disabled);
    border-top-color: var(--md-primary);
    border-radius: 50%;
    animation: spin 0.8s linear infinite;
    margin-right: 8px;
    vertical-align: middle;
}

@keyframes spin {
    to { transform: rotate(360deg); }
}

/* List Items */
.list-item {
    padding: 16px;
    border-bottom: 1px solid var(--md-divider);
    display: flex;
    align-items: center;
    justify-content: space-between;
}

.list-item:last-child {
    border-bottom: none;
}

.list-item-text {
    flex: 1;
}

.list-item-title {
    font-size: 16px;
    font-weight: 500;
    color: var(--md-text-primary);
}

.list-item-subtitle {
    font-size: 14px;
    color: var(--md-text-secondary);
    margin-top: 4px;
}

/* Divider */
.divider {
    height: 1px;
    background: var(--md-divider);
    margin: 16px 0;
}

/* Chip */
.chip {
    display: inline-flex;
    align-items: center;
    padding: 8px 12px;
    border-radius: 16px;
    background: var(--md-surface);
    border: 1px solid var(--md-divider);
    font-size: 14px;
    color: var(--md-text-primary);
    margin: 4px;
}

/* Responsive */
@media (min-width: 600px) {
    body {
        padding: 32px;
    }
    .card {
        padding: 32px;
    }
}
)css";

// Helper class to build Material Design HTML pages
class MaterialPage {
public:
    // Generate HTML page header with Material CSS
    static String getHeader(const String& title, const String& themeColor = "#1976d2") {
        String html = "<!DOCTYPE html><html lang=\"en\"><head>";
        html += "<meta charset=\"UTF-8\">";
        html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no\">";
        html += "<meta name=\"theme-color\" content=\"" + themeColor + "\">";
        html += "<title>" + title + "</title>";
        html += "<style>" + String(MATERIAL_CSS) + "</style>";
        html += "</head><body>";
        return html;
    }

    // Generate app bar
    static String getAppBar(const String& title, const String& subtitle = "") {
        String html = "<div class=\"app-bar\">";
        html += "<h1>" + title + "</h1>";
        if (subtitle.length() > 0) {
            html += "<div class=\"subtitle\">" + subtitle + "</div>";
        }
        html += "</div>";
        return html;
    }

    // Start a card
    static String startCard(const String& title = "") {
        String html = "<div class=\"card\">";
        if (title.length() > 0) {
            html += "<div class=\"card-title\">" + title + "</div>";
        }
        return html;
    }

    // End a card
    static String endCard() {
        return "</div>";
    }

    // Generate HTML page footer
    static String getFooter() {
        return "</body></html>";
    }

    // Form field helper
    static String formField(const String& label, const String& inputHtml) {
        String html = "<div class=\"form-field\">";
        html += "<label>" + label + "</label>";
        html += inputHtml;
        html += "</div>";
        return html;
    }

    // Text input helper
    static String textInput(const String& id, const String& name, bool required = false, const String& placeholder = "") {
        String html = "<input type=\"text\" id=\"" + id + "\" name=\"" + name + "\"";
        if (required) html += " required";
        if (placeholder.length() > 0) html += " placeholder=\"" + placeholder + "\"";
        html += ">";
        return html;
    }

    // Password input helper
    static String passwordInput(const String& id, const String& name, bool required = false) {
        String html = "<input type=\"password\" id=\"" + id + "\" name=\"" + name + "\"";
        if (required) html += " required";
        html += ">";
        return html;
    }

    // Number input helper
    static String numberInput(const String& id, const String& name, int min, int max, int value, bool required = false) {
        String html = "<input type=\"number\" id=\"" + id + "\" name=\"" + name + "\"";
        html += " min=\"" + String(min) + "\" max=\"" + String(max) + "\" value=\"" + String(value) + "\"";
        if (required) html += " required";
        html += ">";
        return html;
    }

    // Button helper
    static String button(const String& text, const String& type = "button", const String& id = "", bool secondary = false) {
        String html = "<button type=\"" + type + "\"";
        if (id.length() > 0) html += " id=\"" + id + "\"";
        if (secondary) html += " class=\"btn-secondary\"";
        html += ">" + text + "</button>";
        return html;
    }

    // Status message helper
    static String statusMessage(const String& id, const String& type = "info") {
        return "<div id=\"" + id + "\" class=\"status " + type + "\"></div>";
    }

    // List item helper
    static String listItem(const String& title, const String& subtitle = "", const String& action = "") {
        String html = "<div class=\"list-item\">";
        html += "<div class=\"list-item-text\">";
        html += "<div class=\"list-item-title\">" + title + "</div>";
        if (subtitle.length() > 0) {
            html += "<div class=\"list-item-subtitle\">" + subtitle + "</div>";
        }
        html += "</div>";
        if (action.length() > 0) {
            html += action;
        }
        html += "</div>";
        return html;
    }
};

#endif
