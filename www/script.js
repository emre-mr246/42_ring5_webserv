console.log('Welcome to Hogwarts!');

document.addEventListener('DOMContentLoaded', function() {
    const links = document.querySelectorAll('a');
    links.forEach(function(link) {
        link.addEventListener('mouseenter', function() {
            this.style.textShadow = '0 0 8px #ffd700';
        });
        link.addEventListener('mouseleave', function() {
            this.style.textShadow = 'none';
        });
    });

    const heading = document.querySelector('h1');
    if (heading) {
        heading.style.transition = 'all 0.3s';
    }
});
