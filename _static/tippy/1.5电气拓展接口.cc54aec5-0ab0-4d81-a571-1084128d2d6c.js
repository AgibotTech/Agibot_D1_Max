selector_to_html = {"a[href=\"#id3\"]": "<h2 class=\"tippy-header\" style=\"margin-top: 0;\">1.5.5 \u4e32\u53e3<a class=\"headerlink\" href=\"#id3\" title=\"Link to this heading\">\uf0c1</a></h2><p><img alt=\"image\" src=\"_images/Female_Aviation_Connector5.png\"/></p>", "a[href=\"#id1\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">1.5 \u7535\u6c14\u62d3\u5c55\u63a5\u53e3<a class=\"headerlink\" href=\"#id1\" title=\"Link to this heading\">\uf0c1</a></h1><p><img alt=\"image\" src=\"_images/Electrical_Expansion_Interface.png\"/></p>", "a[href=\"#usb\"]": "<h2 class=\"tippy-header\" style=\"margin-top: 0;\">1.5.6 USB\u53e3<a class=\"headerlink\" href=\"#usb\" title=\"Link to this heading\">\uf0c1</a></h2><p><img alt=\"image\" src=\"_images/Female_Aviation_Connector6.png\"/></p>", "a[href=\"#v\"]": "<h2 class=\"tippy-header\" style=\"margin-top: 0;\">1.5.3 \u7f51\u53e3(24V)<a class=\"headerlink\" href=\"#v\" title=\"Link to this heading\">\uf0c1</a></h2><p><img alt=\"image\" src=\"_images/Female_Aviation_Connector3.png\"/></p>", "a[href=\"#v-10a\"]": "<h2 class=\"tippy-header\" style=\"margin-top: 0;\">1.5.1 \u7535\u6e90\u53e348V(10A)<a class=\"headerlink\" href=\"#v-10a\" title=\"Link to this heading\">\uf0c1</a></h2><p><img alt=\"image\" src=\"_images/Female_Aviation_Connector1.png\"/></p>", "a[href=\"#v-20a\"]": "<h2 class=\"tippy-header\" style=\"margin-top: 0;\">1.5.2 \u7535\u6e90\u53e324V(20A)<a class=\"headerlink\" href=\"#v-20a\" title=\"Link to this heading\">\uf0c1</a></h2><p><img alt=\"image\" src=\"_images/Female_Aviation_Connector2.png\"/></p>", "a[href=\"#id2\"]": "<h2 class=\"tippy-header\" style=\"margin-top: 0;\">1.5.4 \u7f51\u53e3(12V)<a class=\"headerlink\" href=\"#id2\" title=\"Link to this heading\">\uf0c1</a></h2><p><img alt=\"image\" src=\"_images/Female_Aviation_Connector4.png\"/></p>"}
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
