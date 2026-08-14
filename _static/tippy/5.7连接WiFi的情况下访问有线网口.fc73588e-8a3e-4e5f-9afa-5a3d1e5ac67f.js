selector_to_html = {"a[href=\"#wifi\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">5.7 \u8fde\u63a5WiFi\u7684\u60c5\u51b5\u4e0b\u8bbf\u95ee\u6709\u7ebf\u7f51\u53e3<a class=\"headerlink\" href=\"#wifi\" title=\"Link to this heading\">\uf0c1</a></h1><p>\u8fde\u63a5\u8bbe\u5907\u7684AP\u70ed\u70b9\uff1aXG2WIFI_xxxxx\uff0c\u6dfb\u52a0\u5982\u4e0b\u8def\u7531\u53ef\u5b9e\u73b0\u5728\u8fde\u63a5wifi\u7684\u60c5\u51b5\u4e0b\u80fd\u591f\u8bbf\u95ee\u8bbe\u5907\u7684\u6709\u7ebf\u7f51\u53e3\uff1a</p><p>windows\u7cfb\u7edf\uff1a</p>"}
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
