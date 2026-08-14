selector_to_html = {"a[href=\"#sdk\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">2.6 SDK\u83b7\u53d6<a class=\"headerlink\" href=\"#sdk\" title=\"Link to this heading\">\uf0c1</a></h1><h2>way1\uff1a<a class=\"headerlink\" href=\"#way1\" title=\"Link to this heading\">\uf0c1</a></h2><p>\u901a\u8fc7ssh\u8fdb\u5165\u673a\u5668\u72d7\u672c\u4f53\uff0c\u67e5\u8be2\u5bf9\u5e94\u7684\u7248\u672c\u4fe1\u606f\u540e\uff0c\u5c06\u67e5\u8be2\u5230\u7684\u673a\u5668\u72d7\u56fa\u4ef6\u7248\u672c\u53f7\u622a\u56fe\u53d1\u9001\u552e\u540e\u6280\u672f\u652f\u6301\u56e2\u961f\u83b7\u53d6\u5bf9\u5e94\u7684SDK \u5b89\u88c5\u5305\u4e0e\u793a\u4f8b\u4ee3\u7801\uff0c\u67e5\u8be2\u65b9\u5f0f\u5982\u4e0b\uff1a</p>", "a[href=\"#way1\"]": "<h2 class=\"tippy-header\" style=\"margin-top: 0;\">way1\uff1a<a class=\"headerlink\" href=\"#way1\" title=\"Link to this heading\">\uf0c1</a></h2><p>\u901a\u8fc7ssh\u8fdb\u5165\u673a\u5668\u72d7\u672c\u4f53\uff0c\u67e5\u8be2\u5bf9\u5e94\u7684\u7248\u672c\u4fe1\u606f\u540e\uff0c\u5c06\u67e5\u8be2\u5230\u7684\u673a\u5668\u72d7\u56fa\u4ef6\u7248\u672c\u53f7\u622a\u56fe\u53d1\u9001\u552e\u540e\u6280\u672f\u652f\u6301\u56e2\u961f\u83b7\u53d6\u5bf9\u5e94\u7684SDK \u5b89\u88c5\u5305\u4e0e\u793a\u4f8b\u4ee3\u7801\uff0c\u67e5\u8be2\u65b9\u5f0f\u5982\u4e0b\uff1a</p>", "a[href=\"#way2\"]": "<h2 class=\"tippy-header\" style=\"margin-top: 0;\">way2\uff1a<a class=\"headerlink\" href=\"#way2\" title=\"Link to this heading\">\uf0c1</a></h2><p>\u901a\u8fc7\u5982\u4e0bgithub\u5730\u5740\u83b7\u53d6</p>"}
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
