# Architecture Decision Records

This directory contains Architecture Decision Records (ADRs) for the Smart Home Light Controller project.

## What is an ADR?

An Architecture Decision Record (ADR) is a document that captures an important architectural decision made along with its context and consequences.

## Why ADRs?

- **Document decisions**: Keep a history of why certain decisions were made
- **Team alignment**: Help team members understand the reasoning behind choices
- **Onboarding**: Help new team members understand the project's evolution
- **Prevent revisiting**: Avoid rehashing the same discussions repeatedly

## Naming Convention

ADRs should be numbered sequentially and use the following format:

```
NNNN-short-title.md
```

Examples:
- `0001-use-platformio.md`
- `0002-choose-fastled-library.md`
- `0003-web-server-architecture.md`

## How to Create a New ADR

1. Copy the template:
   ```bash
   cp docs/adr/template.md docs/adr/NNNN-your-title.md
   ```

2. Fill in the sections:
   - Update the title and date
   - Set status to "Proposed" initially
   - Describe the context and problem
   - State your decision
   - List consequences and alternatives

3. Review with team (if applicable)

4. Update status to "Accepted" when decision is finalized

5. Commit to version control

## ADR Lifecycle

- **Proposed**: Decision is under discussion
- **Accepted**: Decision has been agreed upon and implemented
- **Deprecated**: Decision is no longer relevant but kept for historical context
- **Superseded**: Decision has been replaced by a newer ADR (link to the new one)

## Tips for Writing Good ADRs

- **Be concise**: Keep it short and to the point
- **Be specific**: Focus on one decision per ADR
- **Include context**: Explain why this decision matters
- **List alternatives**: Show what else was considered
- **Note trade-offs**: Be honest about the downsides
- **Use simple language**: Avoid jargon when possible

## Template

See [template.md](template.md) for the ADR template.

## Index

List of all ADRs in this project:

- [0001-use-platformio-for-build-system.md](0001-use-platformio-for-build-system.md) - Use PlatformIO as build system
- [0002-use-espasync-wifimanager-for-provisioning.md](0002-use-espasync-wifimanager-for-provisioning.md) - Custom WiFi provisioning with captive portal
- [0003-material-design-framework-for-web-ui.md](0003-material-design-framework-for-web-ui.md) - Use Material Design framework for all web interfaces

(Add new ADRs to this list as they are created)
