const video = document.getElementById('screenVideo');
const cycle = async () => {
    video.pause();
    video.currentTime = 0;
    await new Promise(r => setTimeout(r, 3000)); 
    video.play().catch(() => {});
};
video.onended = cycle;
video.addEventListener('loadedmetadata', cycle, { once: true });