selector_to_html = {"a[href=\"#demo\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">2.7 \u793a\u4f8bDemo\u8fd0\u884c\u6b65\u9aa4<a class=\"headerlink\" href=\"#demo\" title=\"Link to this heading\">\uf0c1</a></h1><p>\u5728\u672c\u5730PC\u7aef\uff0c\u8fdb\u5165\u5230sdk\u6587\u4ef6\u4e2d</p>"}
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
