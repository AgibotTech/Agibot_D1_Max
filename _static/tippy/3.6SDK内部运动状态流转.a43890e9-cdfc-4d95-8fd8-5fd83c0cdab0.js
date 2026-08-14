selector_to_html = {"a[href=\"#sdk\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">3.6 SDK\u5185\u90e8\u8fd0\u52a8\u72b6\u6001\u6d41\u8f6c<a class=\"headerlink\" href=\"#sdk\" title=\"Link to this heading\">\uf0c1</a></h1><p><strong>\u6ce8\u610f\uff1a\u9501\u5b9a\u72b6\u6001\u4e0b\u5207\u5230\u5176\u4ed6\u72b6\u6001(\u7ad9\u7acb\u3001\u530d\u5310\u3001\u5367\u5012)\u4e0b\u5c31\u7b97\u89e3\u9501\u3002</strong></p><p><em>\u5f53\u4e0b\u53d1\u7ad9\u7acb\u547d\u4ee4\u65f6\uff0c\u7ad9\u7acb\u8fc7\u7a0b\u4e2d\u4f1a\u4e0a\u62a5\u7ad9\u7acb\u72b6\u6001\uff0c\u5f53\u5b8c\u6210\u7ad9\u7acb\u540e\u4f1a\u6839\u636e\u5f53\u524d\u7684\u6a21\u5f0f\u81ea\u52a8\u5207\u6362\u5230\u901a\u7528\u72b6\u6001\u6216\u539f\u5730\u72b6\u6001\u6216\u767b\u9636\u72b6\u6001</em></p>", "a[href=\"#id1\"]": "<h2 class=\"tippy-header\" style=\"margin-top: 0;\">SDK\u6a21\u5f0f\u5207\u6362<a class=\"headerlink\" href=\"#id1\" title=\"Link to this heading\">\uf0c1</a></h2><p><strong>\u5f53\u8fdb\u884c\u6a21\u5f0f\u5207\u6362\u65f6\uff0c\u72d7\u4f1a\u81ea\u52a8\u8fdb\u884c\u7ad9\u7acb\uff1b\u7ad9\u7acb\u5b8c\u6210\u540e\u4f1a\u4e0a\u62a5\u76f8\u5e94\u7684\u72b6\u6001\u3002</strong></p>", "a[href=\"#id2\"]": "<h2 class=\"tippy-header\" style=\"margin-top: 0;\">SDK\u547d\u4ee4\u4e0b\u53d1<a class=\"headerlink\" href=\"#id2\" title=\"Link to this heading\">\uf0c1</a></h2><p>\u56fe\u4e2d\u8868\u793a\u4e24\u79cd\u547d\u4ee4\u4e0b\u53d1\u8bbe\u8ba1</p><p><img alt=\"\u56fe\u7247\" src=\"_images/SDK-SendCmd.png\"/></p>"}
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
