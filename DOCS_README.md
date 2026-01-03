# Riverlabs Documentation

This repository contains documentation for the Riverlabs suite of environmental sensors, built with [MkDocs Material](https://squidfunk.github.io/mkdocs-material/).

## Local Development

### Prerequisites

- Python 3.8 or higher
- pip

### Setup

1. Install dependencies:

```bash
pip install -r requirements.txt
```

Or using a virtual environment:

```bash
python -m venv .venv
source .venv/bin/activate  # On Windows: .venv\Scripts\activate
pip install -r requirements.txt
```

### Preview Documentation Locally

Start the development server with live reload:

```bash
mkdocs serve
```

The documentation will be available at http://127.0.0.1:8000/

Any changes you make to the markdown files will automatically reload in your browser.

### Building Documentation

To build the static site:

```bash
mkdocs build
```

This will create a `site/` directory with the generated HTML.

## Deployment

### GitHub Pages

To deploy to GitHub Pages:

```bash
mkdocs gh-deploy
```

This will build the site and push it to the `gh-pages` branch.

## Documentation Structure

```
docs/
├── index.md                 # Home page (English)
├── installation.md          # Installation guide
├── arduino.md              # Arduino setup
├── upload.md               # Uploading code
├── techspec.md             # Technical specifications
├── SDcard.md               # SD card setup
├── start.md                # Getting started
├── thingsboard.md          # ThingsBoard integration
├── xbee3G.md               # XBee 3G setup
├── images/                 # Images and diagrams
└── es/                     # Spanish translations
    ├── index.md
    ├── installation.md
    └── ...
```

## Features

- 🎨 **Material Design** - Clean, modern interface based on Google's Material Design
- 🌓 **Dark Mode** - Automatic light/dark theme switching
- 🔍 **Full-text Search** - Fast client-side search
- 📱 **Mobile Optimized** - Responsive design works on all devices
- 🌐 **Multi-language** - English and Spanish versions
- 📝 **Markdown Extensions** - Admonitions, tabs, code highlighting, and more
- ⚡ **Fast** - Static site generation for optimal performance

## Contributing

When contributing to the documentation:

1. Make changes in a new branch
2. Preview your changes locally with `mkdocs serve`
3. Commit and push your branch
4. Create a pull request

## License

[Add license information]

## Contact

For questions or support, contact Riverlabs Ltd at info@riverlabs.uk
