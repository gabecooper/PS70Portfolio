(function () {
  function muteVideo(video) {
    if (video.muted) return;
    video.muted = true;
    video.defaultMuted = true;
    video.setAttribute("muted", "");
  }

  function wireVideo(video) {
    if (video.dataset.ps70Muted === "1") return;
    video.dataset.ps70Muted = "1";
    muteVideo(video);
    video.addEventListener("volumechange", function () {
      muteVideo(video);
    });
  }

  function muteAllVideos() {
    document.querySelectorAll("video").forEach(wireVideo);
  }

  muteAllVideos();

  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", muteAllVideos);
  }

  new MutationObserver(muteAllVideos).observe(document.documentElement, {
    childList: true,
    subtree: true
  });
})();
