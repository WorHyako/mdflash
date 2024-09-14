#ifndef CSS_STYLES_H
#define CSS_STYLES_H

const char *github_markdown_css = "\
    textview {\
        font-family: -apple-system,BlinkMacSystemFont,Segoe UI,Helvetica,Arial,sans-serif,Apple Color Emoji,Segoe UI Emoji;\
        font-size: 16px;\
        color: #24292e;\
        background-color: #ffffff;\
    }\
    \
    textview text {\
        background-color: #ffffff;\
    }\
    \
    textview text:selected {\
        background-color: #0366d6;\
        color: #ffffff;\
    }\
    \
    .heading {\
        font-weight: bold;\
        font-size: 24px;\
        margin-top: 24px;\
        margin-bottom: 16px;\
    }\
    \
    .code {\
        font-family: SFMono-Regular,Consolas,Liberation Mono,Menlo,monospace;\
        font-size: 85%;\
        background-color: #f6f8fa;\
        border-radius: 3px;\
        padding: 16px;\
        margin-bottom: 16px;\
    }\
";

#endif // CSS_STYLES_H
