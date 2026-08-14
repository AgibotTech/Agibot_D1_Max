selector_to_html = {"a[href=\"#id1\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">4.2 \u6fc0\u5149\u96f7\u8fbe\u6570\u636e<a class=\"headerlink\" href=\"#id1\" title=\"Link to this heading\">\uf0c1</a></h1><h2>4.2.1 ROS2\u63a5\u53e3\u5b9a\u4e49<a class=\"headerlink\" href=\"#ros2\" title=\"Link to this heading\">\uf0c1</a></h2><p><strong>\u524d\u6fc0\u5149\u96f7\u8fbeIP: 192.168.1.102</strong><br/>\n<strong>\u540e\u6fc0\u5149\u96f7\u8fbeIP: 192.168.2.102</strong></p>", "a[href=\"#ros2\"]": "<h2 class=\"tippy-header\" style=\"margin-top: 0;\">4.2.1 ROS2\u63a5\u53e3\u5b9a\u4e49<a class=\"headerlink\" href=\"#ros2\" title=\"Link to this heading\">\uf0c1</a></h2><p><strong>\u524d\u6fc0\u5149\u96f7\u8fbeIP: 192.168.1.102</strong><br/>\n<strong>\u540e\u6fc0\u5149\u96f7\u8fbeIP: 192.168.2.102</strong></p>", "a[href=\"#id2\"]": "<h2 class=\"tippy-header\" style=\"margin-top: 0;\">4.2.2 \u83b7\u53d6\u6fc0\u5149\u96f7\u8fbe\u6570\u636e<a class=\"headerlink\" href=\"#id2\" title=\"Link to this heading\">\uf0c1</a></h2>"}
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
