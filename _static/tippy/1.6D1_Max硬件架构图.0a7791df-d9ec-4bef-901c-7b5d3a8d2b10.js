selector_to_html = {"a[href=\"#d1-max\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">1.6 D1 Max\u786c\u4ef6\u67b6\u6784\u56fe<a class=\"headerlink\" href=\"#d1-max\" title=\"Link to this heading\">\uf0c1</a></h1><p><img alt=\"image\" src=\"_images/Hardware_Architecture.png\"/></p>"}
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
