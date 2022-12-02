#ifndef BE5C0318_54F7_493C_9302_78CF2AFD455D
#define BE5C0318_54F7_493C_9302_78CF2AFD455D
#include <string>
#ifndef __HELPER_UI_H__
#    define __HELPER_UI_H__

#    include <QString>
#    include <optional>

class QJsonDocument;
class QWidget;
class HelperUi
{
  public:
    static QString loadStyleSheetFromResource(const QString& resourcePath);
    static QString formatTextColor(const QString& text, const QString& color);
    /**
     * @brief NOTE all QJson* are implictly shared
     * @param filePath
     * @return
     */
    static std::optional<QJsonDocument> loadJson(const QString& filePath);

    static std::optional<QString> installFont(const QString& fontPath);

    static void makeWidgetTransparent(QWidget* widget,
                                      bool ignoreMouseEvent = false);

    static inline auto wStringToString(std::wstring const& wstring)
    {
        return std::string(wstring.begin(), wstring.end());
    }
};

#endif  // __HELPER_UI_H__

#endif /* BE5C0318_54F7_493C_9302_78CF2AFD455D */
