/**
 * PS70 portfolio slideshow + optional syntax highlighting.
 * Usage: PS70Slideshow.init("rootId", "prevId", "nextId");
 *        PS70Slideshow.initAll([{ root, prev, next }, ...]);
 */
(function (global) {
  function initSlideshow(rootId, prevId, nextId) {
    var root = document.getElementById(rootId);
    if (!root) return;

    var slides = root.querySelectorAll(".mvp-slide");
    if (!slides.length) return;

    var slideIndex = 1;
    var prevBtn = document.getElementById(prevId);
    var nextBtn = document.getElementById(nextId);
    if (!prevBtn || !nextBtn) return;

    function showSlides() {
      var i;
      if (slideIndex > slides.length) slideIndex = 1;
      if (slideIndex < 1) slideIndex = slides.length;

      for (i = 0; i < slides.length; i++) {
        slides[i].style.display = "none";
        var v = slides[i].querySelector("video");
        if (v) {
          v.pause();
          try { v.currentTime = 0; } catch (e) {}
        }
      }

      slides[slideIndex - 1].style.display = "flex";
      var active = slides[slideIndex - 1].querySelector("video");
      if (active) active.play().catch(function () {});
    }

    prevBtn.addEventListener("click", function () { slideIndex--; showSlides(); });
    nextBtn.addEventListener("click", function () { slideIndex++; showSlides(); });
    showSlides();
  }

  function initAll(configs) {
    if (!configs || !configs.length) return;
    configs.forEach(function (cfg) {
      initSlideshow(cfg.root, cfg.prev, cfg.next);
    });
  }

  function initCodeBlocks() {
    if (!global.hljs) return;
    document.querySelectorAll("pre.code-block code").forEach(function (el) {
      try { global.hljs.highlightElement(el); } catch (e) {}
    });
    var esp32 = document.getElementById("esp32Code");
    if (esp32) {
      try { global.hljs.highlightElement(esp32); } catch (e) {}
    }
  }

  global.PS70Slideshow = {
    init: initSlideshow,
    initAll: initAll,
    initCodeBlocks: initCodeBlocks
  };
})(window);
