selector_to_html = {"a[href=\"#id1\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">5.4 \u65e5\u5fd7\u67e5\u770b<a class=\"headerlink\" href=\"#id1\" title=\"Link to this heading\">\uf0c1</a></h1><p><img alt=\"\u56fe\u7247\" src=\"_images/Controler_Error_Log1.png\"/></p><p>\u6253\u5f00\u8bbe\u7f6e\u4e2d\u5fc3\uff0c\u53ef\u67e5\u770b\u8bbe\u5907\u5f02\u5e38\u65e5\u5fd7</p>"}
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
