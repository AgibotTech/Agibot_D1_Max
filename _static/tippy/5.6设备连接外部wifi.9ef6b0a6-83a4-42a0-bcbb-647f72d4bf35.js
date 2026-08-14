selector_to_html = {"a[href=\"#wifi\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">5.6 \u8bbe\u5907\u8fde\u63a5\u5916\u90e8wifi<a class=\"headerlink\" href=\"#wifi\" title=\"Link to this heading\">\uf0c1</a></h1><p><img alt=\"\u56fe\u7247\" src=\"_images/Connect_to_External_WiFi.png\"/></p>"}
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
