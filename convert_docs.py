#!/usr/bin/env python3
"""
Script to convert Jekyll markdown files to MkDocs Material format
"""

import os
import re
from pathlib import Path

def convert_jekyll_to_mkdocs(content, title=""):
    """Convert Jekyll-specific syntax to MkDocs Material syntax"""
    
    # Remove Jekyll front matter
    content = re.sub(r'^---\n.*?\n---\n', '', content, flags=re.DOTALL)
    
    # Add title as H1 if provided and not already present
    if title and not content.strip().startswith('#'):
        content = f"# {title}\n\n{content}"
    
    # Convert Jekyll includes (remove them as they're not directly portable)
    content = re.sub(r'\{%\s*include.*?%\}', '', content)
    
    # Convert note blocks
    content = re.sub(
        r'\{%\s*include\s+note\.html\s+content="(.*?)"\s*%\}',
        r'!!! note\n    \1',
        content
    )
    
    # Convert warning blocks
    content = re.sub(
        r'\{%\s*include\s+warning\.html\s+content="(.*?)"\s*%\}',
        r'!!! warning\n    \1',
        content
    )
    
    # Convert tip blocks
    content = re.sub(
        r'\{%\s*include\s+tip\.html\s+content="(.*?)"\s*%\}',
        r'!!! tip\n    \1',
        content
    )
    
    # Convert callout blocks
    content = re.sub(
        r'\{%\s*include\s+callout\.html\s+content="(.*?)"\s*%\}',
        r'!!! info\n    \1',
        content
    )
    
    # Fix image references - ensure they're on their own line
    content = re.sub(
        r'<img\s+src="([^"]+)"\s+alt="([^"]*)"[^>]*>',
        r'![\2](\1)',
        content
    )
    
    # Convert HTML center tags
    content = re.sub(r'<center>(.*?)</center>', r'\1', content, flags=re.DOTALL)
    
    # Convert div rows and cols (remove them, keep content)
    content = re.sub(r'<div class="row">', '', content)
    content = re.sub(r'<div class="col-md-\d+">', '', content)
    content = re.sub(r'</div>', '', content)
    
    # Clean up multiple blank lines
    content = re.sub(r'\n{3,}', '\n\n', content)
    
    return content.strip()

def extract_title_from_front_matter(content):
    """Extract title from Jekyll front matter"""
    match = re.search(r'^---\n.*?title:\s*"?([^"\n]+)"?.*?\n---', content, re.DOTALL)
    if match:
        return match.group(1).strip()
    return ""

def process_files(source_dir, dest_dir, dest_subdir=""):
    """Process all markdown files in source directory"""
    source_path = Path(source_dir)
    dest_path = Path(dest_dir) / dest_subdir
    dest_path.mkdir(parents=True, exist_ok=True)
    
    for md_file in source_path.glob("*.md"):
        print(f"Processing {md_file.name}...")
        
        with open(md_file, 'r', encoding='utf-8') as f:
            content = f.read()
        
        title = extract_title_from_front_matter(content)
        converted_content = convert_jekyll_to_mkdocs(content, title)
        
        # Determine output filename
        output_file = dest_path / md_file.name
        
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(converted_content)
        
        print(f"  → Created {output_file}")

if __name__ == "__main__":
    base_dir = "/Users/tslr/Projects/work/Riverlabs"
    source_dir = f"{base_dir}/docs/pages"
    dest_dir = f"{base_dir}/docs_new"
    
    # Process English pages
    print("Processing English pages...")
    english_files = [f for f in os.listdir(source_dir) if f.endswith('.md') and not f.endswith('_es.md')]
    
    for filename in english_files:
        filepath = Path(source_dir) / filename
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
        
        title = extract_title_from_front_matter(content)
        converted_content = convert_jekyll_to_mkdocs(content, title)
        
        output_file = Path(dest_dir) / filename
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(converted_content)
        
        print(f"  → Created {output_file}")
    
    # Process Spanish pages
    print("\nProcessing Spanish pages...")
    spanish_files = [f for f in os.listdir(source_dir) if f.endswith('_es.md')]
    
    for filename in spanish_files:
        filepath = Path(source_dir) / filename
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
        
        title = extract_title_from_front_matter(content)
        converted_content = convert_jekyll_to_mkdocs(content, title)
        
        # Remove _es suffix for cleaner Spanish directory
        output_filename = filename.replace('_es.md', '.md')
        output_file = Path(dest_dir) / 'es' / output_filename
        
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(converted_content)
        
        print(f"  → Created {output_file}")
    
    print("\n✅ Conversion complete!")
