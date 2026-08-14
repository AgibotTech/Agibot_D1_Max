selector_to_html = {"a[href=\"#imu\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">4.5 IMU\u6570\u636e<a class=\"headerlink\" href=\"#imu\" title=\"Link to this heading\">\uf0c1</a></h1><p>\u76ee\u524d\u7cfb\u7edf\u4e2d\u6709\u4e09\u4e2aIMU\uff082\u96f7\u8fbe+1\u72ec\u7acb\uff09\uff0c\u901a\u8fc7topic\u53d1\u51fa\u6570\u636e\uff0c\u6807\u51c6ROS2\u7684\u6570\u636e\u7c7b\u578b<code class=\"docutils literal notranslate\"><span class=\"pre\">sensor_msgs::msg::Imu</span></code><br/>\n\u524d\u96f7\u8fbeIMU\uff1a<code class=\"docutils literal notranslate\"><span class=\"pre\">/front_lidar/imu</span></code><br/>\n\u540e\u96f7\u8fbeIMU\uff1a<code class=\"docutils literal notranslate\"><span class=\"pre\">/rear_lidar/imu</span></code><br/>\n\u72ec\u7acbIMU\uff1a<code class=\"docutils literal notranslate\"><span class=\"pre\">/imu_driver/imu_central</span></code><br/>\n<strong>\u6ce8\uff1a\u72ec\u7acbIMU\u6d89\u53ca\u786c\u4ef6\u6539\u52a8\u7684\u95ee\u9898\uff0c\u82e5\u65e0\u6cd5\u83b7\u53d6imu\u6570\u636e\uff0c\u8bf7\u8054\u7cfb\u552e\u540e\u670d\u52a1\u4eba\u5458</strong></p>"}
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
