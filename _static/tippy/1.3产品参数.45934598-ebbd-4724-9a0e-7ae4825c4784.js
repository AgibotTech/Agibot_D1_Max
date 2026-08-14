selector_to_html = {"a[href=\"#id1\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">1.3 \u4ea7\u54c1\u53c2\u6570<a class=\"headerlink\" href=\"#id1\" title=\"Link to this heading\">\uf0c1</a></h1><p><strong>\u6ce8\uff1a\u4ee5\u4e0a\u53c2\u6570\u4e3a\u5b9e\u9a8c\u5ba4\u6d4b\u8bd5\u6570\u636e\uff0c\u5b9e\u9645\u8868\u73b0\u53ef\u80fd\u56e0\u4f7f\u7528\u73af\u5883\u3001\u64cd\u4f5c\u65b9\u5f0f\u7b49\u56e0\u7d20\u6709\u6240\u5dee\u5f02\uff0c\u8bf7\u4ee5\u5b9e\u9645\u4e3a\u51c6</strong></p><p><img alt=\"\u56fe\u7247\" src=\"_images/Product_Parameters1.png\"/></p>"}
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
