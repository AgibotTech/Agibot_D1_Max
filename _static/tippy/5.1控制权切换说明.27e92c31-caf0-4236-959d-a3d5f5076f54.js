selector_to_html = {"a[href=\"#id1\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">5.1 \u63a7\u5236\u6743\u5207\u6362\u8bf4\u660e<a class=\"headerlink\" href=\"#id1\" title=\"Link to this heading\">\uf0c1</a></h1><p><strong>\u529f\u80fd\u8bf4\u660e\uff1a</strong>\n\u63a7\u5236\u6743\u5207\u6362\u529f\u80fd\u4e3b\u8981\u7528\u6765\u5728\u8fdb\u884csdk\u63a7\u5236\u9636\u6bb5\uff0c\u5f53\u51fa\u73b0\u4e00\u4e9b\u7d27\u6025\u60c5\u51b5\u6216\u9700\u8981\u4eba\u5de5\u4ecb\u5165\u5904\u7406\u7684\u573a\u666f\u3002</p><p><strong>\u63a7\u5236\u539f\u5219\uff1a</strong></p>"}
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
