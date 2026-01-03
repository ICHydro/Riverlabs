// Make the logo link to riverlabs.uk website
document.addEventListener('DOMContentLoaded', function() {
    const logoLink = document.querySelector('.md-header__button.md-logo');
    if (logoLink) {
        logoLink.href = 'https://riverlabs.uk/';
        logoLink.target = '_blank';
        logoLink.rel = 'noopener noreferrer';
    }
});
