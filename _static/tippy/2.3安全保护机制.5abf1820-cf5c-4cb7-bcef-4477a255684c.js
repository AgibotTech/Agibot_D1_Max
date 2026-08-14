selector_to_html = {"a[href=\"#id3\"]": "<h2 class=\"tippy-header\" style=\"margin-top: 0;\">2.3.2 \u8f6f\u6025\u505c<a class=\"headerlink\" href=\"#id3\" title=\"Link to this heading\">\uf0c1</a></h2><p><strong>\u4eceV0.0.6\u7248\u672csdk\u8d77\uff0c\u652f\u6301\u9065\u63a7\u5668\u548csdk\u63a7\u5236\u6743\u5207\u6362\u529f\u80fd\uff0c\u82e5\u8981\u5b9e\u73b0\u5728sdk\u63a7\u5236\u9636\u6bb5\u901a\u8fc7\u9065\u63a7\u5668\u89e6\u53d1\u8bbe\u5907\u8f6f\u6025\u505c\uff0c\u5fc5\u987b\u5f97\u5728sdk\u83b7\u53d6\u63a7\u5236\u6743\u540e\uff0c\u70b9\u5f00app\uff0c\u7136\u540e\u4fbf\u53ef\u4ee5\u968f\u65f6\u4f7f\u7528\u9065\u63a7\u5668\u53f3\u4e0a\u65b9\u7ea2\u8272\u6025\u505c\u6309\u94ae\u89e6\u53d1\u8f6f\u6025\u505c\uff0c\u89e6\u53d1\u8f6f\u6025\u505c\u540e\uff0c\u673a\u5668\u72d7\u663e\u793a\u7ea2\u706f\uff0c\u4fdd\u6301\u5f53\u524d\u59ff\u6001\uff0c\u4e0d\u518d\u54cd\u5e94\u4efb\u4f55\u63a7\u5236\u6307\u4ee4</strong></p>", "a[href=\"#id1\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">2.3 \u5b89\u5168\u4fdd\u62a4\u673a\u5236<a class=\"headerlink\" href=\"#id1\" title=\"Link to this heading\">\uf0c1</a></h1><h2>2.3.1 \u786c\u6025\u505c<a class=\"headerlink\" href=\"#id2\" title=\"Link to this heading\">\uf0c1</a></h2>", "a[href=\"#id4\"]": "<h2 class=\"tippy-header\" style=\"margin-top: 0;\">2.3.3 \u5185\u90e8\u5f02\u5e38\u4fdd\u62a4(\u81ea\u52a8\u89e6\u53d1)<a class=\"headerlink\" href=\"#id4\" title=\"Link to this heading\">\uf0c1</a></h2><p>\u4ee5\u4e0b\u60c5\u51b5\u5c06\u89e6\u53d1\u5185\u90e8\u4fdd\u62a4\u673a\u5236\uff0c\u673a\u5668\u4eba\u81ea\u52a8\u505c\u8f66\u5e76\u7f13\u6162\u7740\u5730</p>", "a[href=\"#id2\"]": "<h2 class=\"tippy-header\" style=\"margin-top: 0;\">2.3.1 \u786c\u6025\u505c<a class=\"headerlink\" href=\"#id2\" title=\"Link to this heading\">\uf0c1</a></h2>"}
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
