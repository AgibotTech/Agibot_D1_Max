selector_to_html = {"a[href=\"#sdk\"]": "<h1 class=\"tippy-header\" style=\"margin-top: 0;\">3.5 SDK\u63a7\u5236\u6743\u5f52\u5c5e<a class=\"headerlink\" href=\"#sdk\" title=\"Link to this heading\">\uf0c1</a></h1><h2>SDK\u4e0eAPP\u63a7\u5236\u6743\u95ee\u9898\u8be6\u89e3<a class=\"headerlink\" href=\"#sdkapp\" title=\"Link to this heading\">\uf0c1</a></h2><p><strong>\u539f\u5219</strong></p><p><strong>1.APP\u7aef\u88ab\u5141\u8bb8\u62a2SDK\u7aef\u7684\u63a7\u5236\u6743\uff1b</strong></p>", "a[href=\"#sdk-appsdk-app-appsdk\"]": "<h3 class=\"tippy-header\" style=\"margin-top: 0;\">\u60c5\u51b52\uff1aSDK\u5148\u8fde\u63a5\uff0cAPP\u540e\u8fde\u63a5\u3002\u63a7\u5236\u6743\u5728SDK\u7aef\uff0cAPP\u65e0\u6cd5\u63a7\u5236\uff0c\u4f46\u662fAPP\u53ef\u4ee5\u5f3a\u884c\u62a2\u593aSDK\u63a7\u5236\u6743\u3002<a class=\"headerlink\" href=\"#sdk-appsdk-app-appsdk\" title=\"Link to this heading\">\uf0c1</a></h3><p><img alt=\"\u56fe\u7247\" src=\"_images/SDK-Control_zh.png\"/></p>", "a[href=\"#sdkapp\"]": "<h2 class=\"tippy-header\" style=\"margin-top: 0;\">SDK\u4e0eAPP\u63a7\u5236\u6743\u95ee\u9898\u8be6\u89e3<a class=\"headerlink\" href=\"#sdkapp\" title=\"Link to this heading\">\uf0c1</a></h2><p><strong>\u539f\u5219</strong></p><p><strong>1.APP\u7aef\u88ab\u5141\u8bb8\u62a2SDK\u7aef\u7684\u63a7\u5236\u6743\uff1b</strong></p>", "a[href=\"#app-sdkapp-sdk\"]": "<h3 class=\"tippy-header\" style=\"margin-top: 0;\">\u60c5\u51b51\uff1aAPP\u5148\u8fde\u63a5\uff0cSDK\u540e\u8fde\u63a5\u3002\u63a7\u5236\u6743\u5728APP\u7aef\uff0cSDK\u65e0\u6cd5\u63a7\u5236\u3002<a class=\"headerlink\" href=\"#app-sdkapp-sdk\" title=\"Link to this heading\">\uf0c1</a></h3><p><img alt=\"\u56fe\u7247\" src=\"_images/APP-Control_zh.png\"/></p>"}
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
