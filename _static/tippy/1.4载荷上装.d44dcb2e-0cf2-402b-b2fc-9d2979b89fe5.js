selector_to_html = {"a[href=\"#id3\"]": "<h2 class=\"tippy-header\" style=\"margin-top: 0;\">1.4.2 \u91cd\u5fc3\u9650\u5236<a class=\"headerlink\" href=\"#id3\" title=\"Link to this heading\">\uf0c1</a></h2><p>\u4e3a\u907f\u514dD1 Max\u673a\u8eab\u4e0a\u52a0\u88c5\u4e0a\u88c5\u8f7d\u8377\u4e4b\u540e\uff0c\u5728\u5404\u7c7b\u5730\u5f62\u8fd0\u52a8\u8fc7\u7a0b\u4e2d\uff0c\u817f\u90e8\u4e0e\u4e0a\u88c5\u8f7d\u8377\u53d1\u751f\u78b0\u649e\u5e72\u6d89\uff0c\u5bf9D1 Max\u7684\u4e0a\u88c5\u7a7a\u95f4\u7ed9\u51fa\u5982\u4e0b\u63a8\u8350\u793a\u610f\uff0c\u5177\u4f53\u5c3a\u5bf8\u4e3a\u957f40cm\uff0c\u5bbd22cm\uff0c\u9ad824cm\uff0c\u4f4d\u7f6e\u5728\u673a\u8eab\u80cc\u90e8\u5c45\u4e2d\u4f4d\u7f6e\uff0c\u5982\u4e0b\u56fe\u6240\u793a\uff1a</p><p><img alt=\"\u56fe\u7247\" src=\"_images/CoG_limitation.png\"/></p>", "a[href=\"#id1\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">1.4 \u8f7d\u8377\u4e0a\u88c5<a class=\"headerlink\" href=\"#id1\" title=\"Link to this heading\">\uf0c1</a></h1><h2>1.4.1 \u8d1f\u8f7d\u5b89\u88c5\u5b54\u4f4d\u56fe<a class=\"headerlink\" href=\"#id2\" title=\"Link to this heading\">\uf0c1</a></h2><p>\u673a\u5668\u72d7\u80cc\u90e8\u7684\u7a7a\u95f4\u5e73\u6574\uff0c\u8d1f\u8f7d25-30kg\uff0c\u80cc\u90e8\u90e8\u7f72\u6709\u53cc\u5bfc\u8f68\uff0c\u4e24\u6761\u5bfc\u8f68\u4e0a\u670914\u4e2aM4\u5b54\u4f4d\u652f\u6301\u4e0a\u88c5\u8f7d\u8377\u4fbf\u6377\u5b89\u88c5\uff0c\u53cc\u5bfc\u8f68\u5177\u4f53\u5c3a\u5bf8\u5982\u4e0b</p><p><img alt=\"\u56fe\u7247\" src=\"_images/Rear-load_Mounting.png\"/></p>", "a[href=\"#id2\"]": "<h2 class=\"tippy-header\" style=\"margin-top: 0;\">1.4.1 \u8d1f\u8f7d\u5b89\u88c5\u5b54\u4f4d\u56fe<a class=\"headerlink\" href=\"#id2\" title=\"Link to this heading\">\uf0c1</a></h2><p>\u673a\u5668\u72d7\u80cc\u90e8\u7684\u7a7a\u95f4\u5e73\u6574\uff0c\u8d1f\u8f7d25-30kg\uff0c\u80cc\u90e8\u90e8\u7f72\u6709\u53cc\u5bfc\u8f68\uff0c\u4e24\u6761\u5bfc\u8f68\u4e0a\u670914\u4e2aM4\u5b54\u4f4d\u652f\u6301\u4e0a\u88c5\u8f7d\u8377\u4fbf\u6377\u5b89\u88c5\uff0c\u53cc\u5bfc\u8f68\u5177\u4f53\u5c3a\u5bf8\u5982\u4e0b</p><p><img alt=\"\u56fe\u7247\" src=\"_images/Rear-load_Mounting.png\"/></p>"}
skip_classes = ["headerlink", "sd-stretched-link"]

window.onload = function () {
    for (const [select, tip_html] of Object.entries(selector_to_html)) {
        const links = document.querySelectorAll(` ${select}`);
        for (const link of links) {
            if (skip_classes.some(c => link.classList.contains(c))) {
                continue;
            }

            tippy(link, {
                content: tip_html,
                allowHTML: true,
                arrow: true,
                placement: 'auto-start', maxWidth: 500, interactive: false,

            });
        };
    };
    console.log("tippy tips loaded!");
};
