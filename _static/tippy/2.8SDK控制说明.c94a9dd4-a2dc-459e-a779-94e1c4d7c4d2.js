selector_to_html = {"a[href=\"#sdk\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">2.8 SDK\u63a7\u5236\u8bf4\u660e<a class=\"headerlink\" href=\"#sdk\" title=\"Link to this heading\">\uf0c1</a></h1><p><img alt=\"image\" src=\"_images/SDK_Control_Instruction.png\"/></p><p><strong>\u6ce8\u610f\uff1a</strong><br/>\n1.\u5f53\u4e0b\u53d1\u7ad9\u7acb\u547d\u4ee4\u65f6\uff0c\u5b8c\u6210\u7ad9\u7acb\u540e\u4f1a\u6839\u636e\u5f53\u524d\u7684\u6a21\u5f0f\u81ea\u52a8\u5207\u6362\u5230\u901a\u7528\u72b6\u6001\u6216\u8005\u539f\u5730\u72b6\u6001<br/>\n2.\u9501\u5b9a\u72b6\u6001\u4e0b\u5207\u5230\u4efb\u610f\u5176\u4ed6\u72b6\u6001\u4e0b\u5c31\u7b97\u89e3\u9501</p>"}
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
