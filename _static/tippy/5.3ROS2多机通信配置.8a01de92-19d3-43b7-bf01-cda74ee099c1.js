selector_to_html = {"a[href=\"#ros2\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">5.3 ROS2\u591a\u673a\u901a\u4fe1\u914d\u7f6e<a class=\"headerlink\" href=\"#ros2\" title=\"Link to this heading\">\uf0c1</a></h1><p>NX\u7cfb\u7edf\u4e2d\u6709\u7edf\u4e00\u914d\u7f6e\u7684\u73af\u5883\u53d8\u91cf\uff1a<code class=\"docutils literal notranslate\"><span class=\"pre\">/opt/runtime/env.bash</span></code>, \u8fd9\u4e2a\u53d8\u91cf\u5728<code class=\"docutils literal notranslate\"><span class=\"pre\">.bashrc</span></code>\u4e2d\u4f1a\u9ed8\u8ba4\u5bfc\u5165</p><p><img alt=\"\u56fe\u7247\" src=\"_images/ROS2_Communication_Config.png\"/></p>"}
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
