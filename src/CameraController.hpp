#pragma once

#include <QObject>

class CameraController : public QObject
{
    Q_OBJECT
public:
    explicit CameraController(QObject *parent = nullptr);

    bool eventFilter(QObject *object, QEvent *event) override;

signals:

public slots:
};
