# Documentation Project Guidelines

## Project Overview

This is a comprehensive documentation migration and enhancement project for Riverlabs environmental sensor loggers. The project involves transitioning from Jekyll to MkDocs Material and significantly improving documentation quality, organization, and user experience.

## Core Philosophy

### User-Centric Approach
- **Prioritize user needs** over technical completeness
- Write for practitioners in the field, not just engineers
- Assume minimal technical background unless in Developer Manual
- Use clear, direct language - avoid jargon where possible
- Include practical examples and real-world scenarios

### Documentation Structure
- **Two-tier system**: User Manual (practical operation) vs Developer Manual (technical implementation)
- **Progressive disclosure**: Start simple, link to deeper content
- **Task-oriented**: Organize by what users need to do, not by system architecture
- Follow the user's journey: Setup → Deploy → Monitor → Maintain

## Style Guidelines

### Writing Style
- **Active voice**: "Press the button" not "The button should be pressed"
- **Direct instructions**: Use numbered steps for procedures
- **Conversational but professional**: Friendly without being informal
- **Concise**: Every word should serve a purpose
- **Scannable**: Use headings, bullets, tables, and callouts liberally

### Formatting Conventions
- Use MkDocs Material admonitions for emphasis:
  - `!!! warning` for critical safety/data loss issues
  - `!!! tip` for helpful suggestions
  - `!!! note` for additional context
  - `!!! danger` for severe warnings
- **Bold** for UI elements and important actions
- `Code formatting` for filenames, commands, and technical terms
- Tables for comparisons and specifications
- Mermaid diagrams for workflows and processes

### Content Principles
- **No checkboxes** - documentation should be guides, not forms
- **Lead with context** - explain why before how
- **Anticipate questions** - address common confusion points
- **Link generously** - connect related content
- **Show, don't just tell** - include examples and screenshots where helpful

## Navigation & Organization

### Information Architecture
```
User Manual/
├── Quick Start (getting started fast)
├── Installation (physical setup)
├── Hardware (specifications, power, maintenance)
├── Configuration (settings and setup)
└── Troubleshooting (problem solving)

Developer Manual/
├── Programming (Arduino, code upload)
├── Hardware (internal components, technical details)
└── Technical Reference (advanced integration)
```

### Navigation UX
- Keep navigation hierarchy shallow (2-3 levels max)
- Use clear, specific page titles (not generic ones like "Overview")
- Maintain logical flow within sections
- Subtle indentation for visual hierarchy (0.15rem to 0.4rem padding)

## Branding & Visual Design

### Current Implementation
- **Logo**: Riverlabs white mono SVG in header (2.5rem height)
- **Logo behavior**: Links to riverlabs.uk (external site)
- **No site title** in header (logo speaks for itself)
- **Right-aligned** header controls (search, theme, repo link)
- **Material Blue** theme (MkDocs Material default with customization)

### Custom Styling Philosophy
- Minimal, professional appearance
- Let content breathe - don't over-style
- Consistent spacing and typography
- Accessibility first

## Content Migration Strategy

### From README Files
When incorporating README content into documentation:
1. **Extract practical information** (setup steps, warnings, specifications)
2. **Remove code-specific details** unless in Developer Manual
3. **Reorganize by user task** not by code structure
4. **Expand terse instructions** into clear step-by-step procedures
5. **Add context and rationale** - READMEs often assume too much knowledge

### Cross-Referencing
- Every README detail should appear somewhere in the docs
- Link between related topics extensively
- Create "See Also" sections at page ends
- Build a web of knowledge, not isolated pages

## Quality Standards

### Before Publishing
- [ ] Read aloud - does it sound natural?
- [ ] Check links - all internal references work?
- [ ] Test procedures - can someone follow these steps?
- [ ] Review warnings - are critical issues clearly highlighted?
- [ ] Scan formatting - is it visually clear and consistent?
- [ ] Verify accuracy - is technical information correct?

### Maintenance Principles
- Documentation is never "done" - it evolves with the product
- User feedback should drive updates
- Keep deprecated content clearly marked or removed
- Update screenshots and examples as UI changes

## Technical Implementation

### MkDocs Configuration
- Material theme with navigation features enabled
- Custom CSS for logo and navigation styling
- Custom JavaScript for external logo link
- GitHub Actions for automated deployment
- Separate site_url for forks vs main repository

### File Naming
- **Lowercase with hyphens**: `first-data-download.md` (not `First_Data_Download.md`)
- **Descriptive names**: `battery-power-guide.md` (not `power.md`)
- **Consistent structure**: match navigation hierarchy

### Git Workflow
- Work on feature branches (like `docs-update-27.12.25`)
- Commit related changes together with clear messages
- Push regularly to enable preview on fork
- Document significant changes in commit messages

## Common Patterns

### Page Structure Template
```markdown
# Page Title

Brief introduction paragraph setting context.

## Overview/Introduction
What this page covers and why it matters.

## Main Content Sections
Step-by-step procedures or organized information.

## Troubleshooting (if applicable)
Common issues related to this topic.

## Next Steps
- Link to related page
- Link to advanced topic
- Link to troubleshooting
```

### Writing Procedures
```markdown
**Step Description:**

1. First specific action
2. Second specific action
   - Sub-detail if needed
   - Expected result

!!! tip
    Helpful advice or time-saving suggestion
```

### Tables for Comparisons
Use tables when comparing options, specifications, or organizing structured data:
```markdown
| Feature | Option A | Option B |
|---------|----------|----------|
| Cost | Lower | Higher |
| Range | 5m | 35m |
```

## Key Decisions & Rationale

### Why MkDocs Material over Jekyll?
- Modern, actively maintained
- Better built-in features (search, navigation)
- Easier customization
- Better mobile experience
- Simpler deployment via GitHub Actions

### Why Two Manuals?
- User Manual: Operational focus - "How do I use this?"
- Developer Manual: Technical focus - "How does this work?"
- Reduces cognitive load - users see only what they need
- Allows different writing styles/depth

### Why Remove Checkboxes?
- Documentation should be guides, not checklists
- Checklists imply interactive forms
- Users can create their own checklists if needed
- Cleaner, more professional appearance

## Avoiding Common Pitfalls

### Don't:
- ❌ Use passive voice ("The button can be pressed...")
- ❌ Assume prior knowledge without linking to explanations
- ❌ Write long paragraphs - break them up
- ❌ Use technical jargon without defining it
- ❌ Create orphan pages with no links to/from them
- ❌ Copy-paste README content verbatim
- ❌ Forget to update navigation when adding/moving pages

### Do:
- ✅ Write step-by-step procedures for tasks
- ✅ Include warnings before critical steps
- ✅ Link to related content generously
- ✅ Use visual hierarchy (headings, bullets, callouts)
- ✅ Test procedures by following them yourself
- ✅ Add context - explain why, not just how
- ✅ Keep Git history clean and meaningful

## Future Considerations

### Potential Enhancements
- Video tutorials for complex procedures
- Interactive troubleshooting flowcharts
- Downloadable PDFs for field reference
- Version-specific documentation (if firmware versions diverge significantly)
- Multi-language support (Spanish already partially present)
- Community contributions section

### Ongoing Maintenance
- Regular review of user feedback
- Update with new firmware features
- Refresh screenshots and examples
- Monitor broken links
- Track documentation gaps

---

## Quick Start for New Sessions

When starting a new conversation about this project:

1. **Context**: "This is the Riverlabs documentation project - migrated from Jekyll to MkDocs Material"
2. **Structure**: "Two manuals: User (practical operation) and Developer (technical details)"
3. **Style**: "Clear, task-oriented, user-friendly writing with admonitions and examples"
4. **Files**: Located in `docs/` directory, deployed via GitHub Actions
5. **Navigation**: Defined in `mkdocs.yml`, organized by user journey
6. **Reference**: This CLAUDE.md file for style and organizational principles

## Contact & Resources

- **Repository**: github.com/ICHydro/Riverlabs (main) / github.com/trowan92/Riverlabs (fork)
- **Live Docs**: trowan92.github.io/Riverlabs/ (preview)
- **MkDocs Material**: https://squidfunk.github.io/mkdocs-material/
- **Product Site**: riverlabs.uk
