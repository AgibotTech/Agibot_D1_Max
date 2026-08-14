selector_to_html = {"a[href=\"#id1\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">1.1 \u4ea7\u54c1\u6982\u8ff0<a class=\"headerlink\" href=\"#id1\" title=\"Link to this heading\">\uf0c1</a></h1><p>\u667a\u5143\u4eff\u751f\u56db\u8db3\u673a\u5668\u4eba D1 Max\u662f\u4e00\u6b3e\u8f6e\u8db3\u5f0f\u4eff\u751f\u56db\u8db3\u673a\u5668\u4eba\uff0c \u5177\u5907\u81ea\u91cd\u8f7b\u3001\u8d1f\u8f7d\u5927\u3001\u7eed\u822a\u957f\u3001\u9632\u62a4\u5f3a\u3001\u8fd0\u52a8\u7075\u6d3b\u7a33\u5b9a\u7b49\u4f18\u52bf\u3002\u673a\u5668\u72d7\u6bcf\u6761\u817f\u914d\u59073\u4e2a\u5173\u8282\u7535\u673a\u548c1\u4e2a\u8f6e\u6bc2\u7535\u673a\uff0c\u914d\u5907\u5149\u5b66\u76f8\u673a\u3001\u6fc0\u5149\u96f7\u8fbe\u3001\u8d85\u58f0\u6ce2\u96f7\u8fbe\u3001IMU\u3001RTK\u6a21\u5757\u7b49\u4f20\u611f\u8bbe\u5907\uff0c\u5185\u90e8\u642d\u8f7d\u9ad8\u7b97\u529b\u5e73\u53f0\uff0c\u7528\u4e8e\u5b9e\u73b0\u8fd0\u52a8\u63a7\u5236\u3001\u81ea\u4e3b\u5bfc\u822a\u5b9a\u4f4d\u3001\u73af\u5883\u4fa6\u6d4b\u7b49\u529f\u80fd\uff0c\u63d0\u4f9b\u4e30\u5bcc\u7684\u4f9b\u7535\u548c\u901a\u4fe1\u63a5\u53e3\uff0c\u652f\u6301\u591a\u54c1\u7c7b\u7684\u4efb\u52a1\u8f7d\u8377\u62d3\u5c55\u3002</p><p><img alt=\"\u56fe\u7247\" src=\"_images/Product_Overview.png\"/></p>"}
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
