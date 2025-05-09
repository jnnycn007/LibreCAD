#ifndef LC_DEVICEOPTIONS_H
#define LC_DEVICEOPTIONS_H

#include <QFrame>

namespace Ui {
    class LC_DeviceOptions;
}

class LC_DeviceOptions : public QFrame{
    Q_OBJECT
public:
    explicit LC_DeviceOptions(QWidget* parent);
    ~LC_DeviceOptions() override;
public slots:
    void save();
private:
    Ui::LC_DeviceOptions* ui = nullptr;
};

#endif // LC_DEVICEOPTIONS_H
