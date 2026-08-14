selector_to_html = {"a[href=\"#id1\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">2.10 \u4f20\u611f\u8bbe\u5907\u5750\u6807\u5b9a\u4e49<a class=\"headerlink\" href=\"#id1\" title=\"Link to this heading\">\uf0c1</a></h1><p>\u5f53\u5404\u4e2a\u5173\u8282\u5747\u4e3a\u96f6\u5ea6\u65f6\uff0c\u5404\u5750\u6807\u7cfb\u5982\u4e0b\u56fe\u3002\u7ea2\u8272\u4e3a x \u8f74\uff0c\u7eff\u8272\u4e3a y \u8f74\uff0c\u84dd\u8272\u4e3a z \u8f74\u3002\u5173\u8282\u7684\u65cb\u8f6c\u8f74\u548c\u65cb\u8f6c\u6b63\u65b9\u5411\u8bf7\u53c2\u8003\u4e0b\u56fe</p><p><img alt=\"image\" src=\"_images/Body-frame_Orientaion.png\"/></p>"}
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
