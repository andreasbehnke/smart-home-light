# Material Design Web Framework

This project uses a custom Material Design-inspired CSS framework for all web interfaces. The framework provides a consistent, mobile-optimized, dark-mode-enabled user experience across all pages.

## Architecture

### Core Components

1. **`src/web_material.h`** - The reusable framework
   - `MATERIAL_CSS` - Complete Material Design CSS (~7KB)
   - `MaterialPage` - Helper class for building pages

2. **Page-specific files** (e.g., `src/portal_material.h`)
   - Import `web_material.h`
   - Use `MATERIAL_CSS` for styling
   - Build page-specific HTML and JavaScript

## Creating a New Page

### Method 1: Using Helper Class (Recommended)

```cpp
#include "web_material.h"

String buildMyPage() {
    String html = MaterialPage::getHeader("My Page Title");
    html += MaterialPage::getAppBar("Smart Home Light", "My Feature");

    // Card with form
    html += MaterialPage::startCard("Configuration");
    html += MaterialPage::formField("Name",
                MaterialPage::textInput("name", "name", true));
    html += MaterialPage::formField("Value",
                MaterialPage::numberInput("value", "value", 0, 100, 50, true));
    html += MaterialPage::button("Save", "submit", "saveBtn");
    html += MaterialPage::endCard();

    // Another card with info
    html += MaterialPage::startCard("Status");
    html += MaterialPage::listItem("Device", "Connected",
                "<span class='chip'>Online</span>");
    html += MaterialPage::listItem("Uptime", "123 seconds");
    html += MaterialPage::endCard();

    html += MaterialPage::getFooter();
    return html;
}
```

### Method 2: Manual HTML (More Control)

```cpp
#include "web_material.h"

String buildMyPage() {
    String html = R"html(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>My Page</title>
    <style>)html";

    html += String(MATERIAL_CSS);

    html += R"html(</style>
</head>
<body>
    <div class="app-bar">
        <h1>Smart Home Light</h1>
        <div class="subtitle">My Feature</div>
    </div>

    <div class="card">
        <div class="card-title">Configuration</div>
        <form id="myForm">
            <div class="form-field">
                <label for="name">Name</label>
                <input type="text" id="name" name="name" required>
            </div>
            <button type="submit">Save</button>
        </form>
    </div>

    <script>
        // Your JavaScript here
    </script>
</body>
</html>)html";

    return html;
}
```

## MaterialPage Helper Methods

### Page Structure

| Method | Description | Returns |
|--------|-------------|---------|
| `getHeader(title, themeColor)` | HTML header with Material CSS | String |
| `getAppBar(title, subtitle)` | Top app bar with title | String |
| `startCard(title)` | Begin a Material card | String |
| `endCard()` | Close a Material card | String |
| `getFooter()` | HTML footer | String |

### Form Elements

| Method | Description | Returns |
|--------|-------------|---------|
| `formField(label, inputHtml)` | Labeled form field wrapper | String |
| `textInput(id, name, required, placeholder)` | Text input field | String |
| `passwordInput(id, name, required)` | Password input field | String |
| `numberInput(id, name, min, max, value, required)` | Number input field | String |

### UI Components

| Method | Description | Returns |
|--------|-------------|---------|
| `button(text, type, id, secondary)` | Material button | String |
| `statusMessage(id, type)` | Status message container (success/error/info) | String |
| `listItem(title, subtitle, action)` | List item with optional action | String |

## Available CSS Classes

### Layout

- `.app-bar` - Top app bar with elevation
- `.card` - Material card with elevation
- `.card-title` - Card title text
- `.form-field` - Form field container
- `.divider` - Horizontal divider line

### Components

- `.btn` - Button (also works with `<button>`)
- `.btn-secondary` - Secondary button style
- `.chip` - Small info chip
- `.list-item` - List item container
- `.list-item-text` - List item text wrapper
- `.list-item-title` - List item main text
- `.list-item-subtitle` - List item secondary text

### Status

- `.status` - Status message container (hidden by default)
- `.status.success` - Success message (green)
- `.status.error` - Error message (red)
- `.status.info` - Info message (blue)

### Utility

- `.loading` - Loading indicator text
- `.spinner` - Loading spinner animation

## Design Tokens (CSS Variables)

### Colors

Light mode (default):
- `--md-primary`: #1976d2 (blue)
- `--md-primary-dark`: #1565c0
- `--md-primary-light`: #42a5f5
- `--md-accent`: #ff4081 (pink)
- `--md-background`: #fafafa
- `--md-surface`: #ffffff
- `--md-error`: #f44336
- `--md-success`: #4caf50

Dark mode (automatic via `prefers-color-scheme`):
- `--md-primary`: #90caf9 (lighter blue)
- `--md-background`: #121212
- `--md-surface`: #1e1e1e

### Typography

- `--md-text-primary`: Main text color
- `--md-text-secondary`: Secondary text color
- `--md-text-disabled`: Disabled text color

### Elevation

- `--md-elevation-1`: Subtle shadow
- `--md-elevation-2`: Card shadow (default)
- `--md-elevation-4`: App bar shadow

## Examples

### Simple Settings Page

```cpp
String html = MaterialPage::getHeader("Settings");
html += MaterialPage::getAppBar("Smart Home Light", "Settings");
html += MaterialPage::startCard("LED Configuration");
html += MaterialPage::formField("Brightness",
            MaterialPage::numberInput("brightness", "brightness", 0, 255, 128, true));
html += MaterialPage::button("Apply", "submit");
html += MaterialPage::statusMessage("status", "info");
html += MaterialPage::endCard();
html += MaterialPage::getFooter();
```

### Status Dashboard

```cpp
String html = MaterialPage::getHeader("Dashboard");
html += MaterialPage::getAppBar("Smart Home Light", "Status Dashboard");

html += MaterialPage::startCard("System Information");
html += MaterialPage::listItem("WiFi", wifiProv.getSSID(),
            "<span class='chip'>" + String(wifiProv.getRSSI()) + " dBm</span>");
html += MaterialPage::listItem("IP Address", wifiProv.getIP());
html += MaterialPage::listItem("Free Memory", String(ESP.getFreeHeap()) + " bytes");
html += "<div class='divider'></div>";
html += MaterialPage::button("Restart", "button", "restartBtn", true);
html += MaterialPage::endCard();

html += MaterialPage::getFooter();
```

### Form with Validation

```cpp
String html = MaterialPage::getHeader("Configure LED Strip");
html += MaterialPage::getAppBar("Smart Home Light", "LED Configuration");

html += MaterialPage::startCard();
html += R"html(
<form id="ledForm">
)html";
html += MaterialPage::formField("LED Count",
            MaterialPage::numberInput("count", "count", 1, 500, 60, true));
html += MaterialPage::formField("Pin Number",
            MaterialPage::numberInput("pin", "pin", 0, 39, 5, true));
html += MaterialPage::button("Save Configuration", "submit", "submitBtn");
html += MaterialPage::statusMessage("status");
html += R"html(
</form>
<script>
document.getElementById('ledForm').addEventListener('submit', async (e) => {
    e.preventDefault();
    const formData = new FormData(e.target);
    const status = document.getElementById('status');

    try {
        const response = await fetch('/api/led/config', {
            method: 'POST',
            body: new URLSearchParams(formData)
        });

        if (response.ok) {
            status.className = 'status success';
            status.textContent = '✓ Configuration saved!';
        } else {
            status.className = 'status error';
            status.textContent = '✗ Failed to save configuration';
        }
    } catch (error) {
        status.className = 'status error';
        status.textContent = '✗ Error: ' + error.message;
    }
});
</script>
)html";
html += MaterialPage::endCard();
html += MaterialPage::getFooter();
```

## Dark Mode

Dark mode is **automatic** and responds to the device's system preference. It uses CSS `prefers-color-scheme: dark` media query. No JavaScript or manual toggle needed.

## Mobile Optimization

The framework is mobile-first and includes:

- Touch-friendly 48px minimum button heights
- 56px input heights for easy tapping
- Responsive breakpoints at 600px
- Proper viewport scaling
- No horizontal scrolling

## Size Considerations

- Material CSS: ~7KB
- Helper class code: ~2KB compiled
- Per-page HTML: ~3-5KB

Total flash usage: ~9KB framework + ~3-5KB per page

## Browser Support

Works on all modern browsers:
- Chrome/Edge (Android, iOS, Desktop)
- Safari (iOS, macOS)
- Firefox (Android, Desktop)

## See Also

- [Architecture Decision Record 0003](adr/0003-material-design-framework-for-web-ui.md)
- [Material Design Guidelines](https://m3.material.io/)
