selector_to_html = {"a[href=\"#d1-max-sdkv0-1-0\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">\u667a\u5143\u9177\u62d3\u56db\u8db3\u673a\u5668\u4ebaD1 Max SDK\u5f00\u53d1\u6307\u5357V0.1.0<a class=\"headerlink\" href=\"#d1-max-sdkv0-1-0\" title=\"Link to this heading\">\uf0c1</a></h1>"}
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
