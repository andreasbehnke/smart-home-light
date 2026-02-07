# 3. Use Material Design Framework for All Web Interfaces

Date: 2026-02-07

## Status

Accepted

## Context

All web interfaces in the project (captive portal, control panels, settings pages) need a CSS framework that:
- **Works well on mobile devices** (primary access method)
- **Supports dark mode** (better UX, less battery drain on AMOLED)
- **Is extremely lightweight** (ESP32 has limited flash storage ~4MB, RAM ~320KB)
- **Can be inlined** (no external CDN access in AP mode or offline scenarios)
- **Looks professional** without heavy JavaScript
- **Is responsive** out of the box
- **Is reusable** across all web pages in the project

HTML pages are embedded directly in the firmware as string constants, so every byte counts. We need a consistent look and feel across all web interfaces.

## Options Considered

### Option 1: Plain Custom CSS (Per Page)
**Pros:**
- Full control per page
- Minimal size (~2KB per page)
- No dependencies

**Cons:**
- Inconsistent UX across pages
- Duplicated effort for each new page
- More maintenance burden
- Not as polished as frameworks
- Mobile optimization needs manual work per page

### Option 2: Tailwind CSS
**Pros:**
- Very popular
- Excellent mobile support
- Good dark mode

**Cons:**
- **Too large** (~50KB+ even minimized)
- Requires build process
- Overkill for simple forms

### Option 3: Bootstrap
**Pros:**
- Well-known
- Good components

**Cons:**
- **Too large** (~25KB CSS + JS)
- jQuery dependency
- Not optimized for embedded systems

### Option 4: Pico.css
**Pros:**
- **Very lightweight** (~10KB minified, ~3KB gzipped)
- **Built-in automatic dark mode** (respects system preference)
- **Semantic HTML** (minimal class usage)
- **Mobile-first responsive design**
- **No JavaScript required**

**Cons:**
- Less control than custom CSS
- Not Material Design aesthetic

### Option 5: Simple.css / Water.css
**Pros:**
- **Extremely lightweight** (~4KB)
- Classless (no classes needed)
- Automatic dark mode

**Cons:**
- Less professional look
- Limited customization
- Not as mobile-optimized as Material Design

### Option 6: Full Material UI / Material Components
**Pros:**
- Excellent mobile UX
- Industry-standard design language
- Complete component library

**Cons:**
- **Way too large** (>200KB)
- JavaScript framework dependency
- Complete overkill for embedded web UI

## Decision

We will use a **custom Material Design-inspired CSS framework** as a shared, reusable foundation for all web pages in the project.

**Rationale:**
1. **Material Design principles** - Industry-standard, familiar design language
2. **Reusable across all pages** - Single CSS file used by captive portal, control UI, settings pages, etc.
3. **Lightweight** - Custom CSS (~7KB) vs full Material UI framework (>200KB)
4. **Mobile-optimized** - Touch-friendly 48px minimum target sizes, proper spacing
5. **Automatic dark mode** - Material dark theme using CSS variables and `prefers-color-scheme`
6. **Professional appearance** - Elevation shadows, cards, proper typography
7. **Full control** - Can optimize for ESP32 constraints
8. **Consistency** - All pages look and feel the same

**Why not full Material Components?**
- Material Components Web: ~200KB (too large)
- Materialize CSS: ~100KB (too large)
- Material Design Lite: ~140KB (too large, deprecated)
- Custom Material Design framework: ~7KB (perfect for ESP32)

## Implementation

### Architecture

The framework is implemented in two parts:

1. **`src/web_material.h`** - Reusable Material Design CSS and helper class
   - Contains `MATERIAL_CSS` constant with all Material Design styles
   - Provides `MaterialPage` helper class with methods to build pages
   - Used by all web pages for consistent styling

2. **Individual page files** (e.g., `src/portal_material.h`)
   - Import `web_material.h`
   - Use `MATERIAL_CSS` for styling
   - Use `MaterialPage` helper methods for consistent structure
   - Only contain page-specific HTML and JavaScript

### Material Design Features

The framework implements these Material Design principles:

- **Material color system** - Primary, accent, surface, background colors
- **Elevation** - Shadows at levels 1, 2, and 4 for depth perception
- **Typography** - Roboto-style system fonts with proper weights
- **Cards** - Elevated surfaces for content grouping
- **App bar** - Consistent header across all pages
- **Touch targets** - 48px minimum for buttons, proper spacing
- **Form controls** - Material-style inputs with focus states
- **Status messages** - Color-coded success/error/info states
- **Responsive design** - Mobile-first with tablet/desktop breakpoints

### Helper Class Usage

The `MaterialPage` class provides methods to build consistent pages:

```cpp
String html = MaterialPage::getHeader("Page Title");
html += MaterialPage::getAppBar("App Name", "Subtitle");
html += MaterialPage::startCard("Card Title");
html += MaterialPage::formField("Label", MaterialPage::textInput("id", "name"));
html += MaterialPage::button("Submit", "submit");
html += MaterialPage::endCard();
html += MaterialPage::getFooter();
```

### Size Impact

- Shared Material CSS: ~7KB
- Helper class code: ~2KB compiled
- Per-page HTML: ~3-5KB (without duplicated CSS)
- **Total overhead**: ~9KB for framework, then ~3-5KB per page
- **Percentage of flash**: ~0.2% for framework
- **Acceptable trade-off** for professional, consistent UX

## Consequences

### Positive

- **Consistent UX** - All web pages share the same look and feel
- **Faster development** - Helper methods speed up creating new pages
- **Better mobile UX** - Touch-optimized, proper spacing out of the box
- **Dark mode everywhere** - Automatic across all pages
- **Professional appearance** - Increases user trust
- **Less maintenance** - Changes to CSS affect all pages
- **Accessibility** - Material Design includes proper contrast ratios
- **Code reuse** - No duplication of CSS across pages

### Negative

- **Initial overhead** - ~9KB for framework (one-time cost)
- **Learning curve** - Team needs to learn framework conventions
- **Memory usage** - CSS loaded from PROGMEM for each page generation
- **Less flexibility** - Framework establishes design constraints

### Trade-offs

We accept the ~9KB framework overhead in exchange for:
- Development speed (no redesigning each page)
- UX consistency (users learn interface once)
- Professional appearance (builds trust)
- Maintainability (single source of truth for styling)

## Future Considerations

If size becomes an issue:
1. Remove unused CSS features (e.g., chips, advanced components)
2. Serve CSS from SPIFFS/LittleFS (requires filesystem partition)
3. Use gzip compression for HTML delivery
4. Consider lazy-loading CSS for infrequently accessed pages

If more advanced features are needed:
1. Add Material Design components incrementally (tabs, dialogs, etc.)
2. Consider JavaScript-based Material Web Components for complex UIs
3. Implement theming system for customizable colors

## References

- [Material Design Guidelines](https://m3.material.io/)
- [Material Design Color System](https://m3.material.io/styles/color/system)
- [Material Design Elevation](https://m3.material.io/styles/elevation/overview)
- [CSS prefers-color-scheme](https://developer.mozilla.org/en-US/docs/Web/CSS/@media/prefers-color-scheme)
- [Mobile-First Design Principles](https://developer.mozilla.org/en-US/docs/Web/Progressive_web_apps/Responsive/Mobile_first)

## Related ADRs

- [0002-use-espasync-wifimanager-for-provisioning](0002-use-espasync-wifimanager-for-provisioning.md) - Uses this framework for captive portal
