selector_to_html = {"a[href=\"#id1\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">5.2 \u81ea\u4e3b\u56de\u5145\u8bf4\u660e<a class=\"headerlink\" href=\"#id1\" title=\"Link to this heading\">\uf0c1</a></h1><p><strong>\u7248\u672c\u8981\u6c42\uff1a</strong>\nrk3588\u7248\u672c\u4e3a: 0.2.4\uff0cOrin NX\u56fa\u4ef6\u7248\u672c\u4e3a: 0.4.5</p><p><strong>\u4f7f\u7528\u8981\u6c42\uff1a</strong>\n\u9700\u642d\u914d\u5145\u7535\u6869\u8fdb\u884c\u4f7f\u7528\uff0c\u5b89\u88c5\u8bf4\u660e\u8be6\u89c1\u201c\u667a\u5143D1 Max\u81ea\u4e3b\u5145\u7535\u4ea7\u54c1\u8bf4\u660e\u4e66\u201d\uff0c\u76ee\u524d\u652f\u6301\"\u65e0\u56fe\u56de\u5145\"\uff0c\u5373\u5c06\u56db\u8db3\u673a\u5668\u4ebaD1 Max\u8bbe\u5907\u6b63\u5bf9\u5145\u7535\u6869\uff0c\u8bbe\u5907\u5934\u90e8\u8ddd\u79bb\u5145\u7535\u68691.5m\u5de6\u53f3\uff0c\u786e\u4fdd\u5728\u76f8\u673a\u89c6\u91ce\u4e2d\u53ef\u5b8c\u6574\u770b\u5230\u5145\u7535\u6869\u7684\u4e8c\u7ef4\u7801\u3002</p>"}
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
