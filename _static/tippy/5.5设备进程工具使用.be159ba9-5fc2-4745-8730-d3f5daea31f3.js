selector_to_html = {"a[href=\"#id1\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">5.5 \u8bbe\u5907\u8fdb\u7a0b\u5de5\u5177\u4f7f\u7528<a class=\"headerlink\" href=\"#id1\" title=\"Link to this heading\">\uf0c1</a></h1><p><strong>\u4f7f\u7528\u8bf4\u660e\uff1a</strong>\n\u5728\u8bbe\u5907\u5185\u90e8\u901a\u8fc7robot-launch\u5de5\u5177\u5bf9\u76f8\u5173\u8fdb\u7a0b\u8fdb\u884c\u7ba1\u7406\uff0c\u5305\u62ec\u67e5\u770b\u8fdb\u7a0b\u8fd0\u884c\u72b6\u6001\u3001\u542f\u52a8\u548c\u505c\u6b62\u8fdb\u7a0b\uff0c\u67e5\u770b\u8fdb\u7a0b\u65e5\u5fd7\u4fe1\u606f\u7b49\u3002<br/>\n<strong>\u683c\u5f0f\u8bf4\u660e\uff1a</strong>\nrobot-launch [OPTIONS] [COMMAND]</p><p><img alt=\"\u56fe\u7247\" src=\"_images/Device_process_Tools_Usage.png\"/></p>"}
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
