#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QWidget>

class Rectangle : public QWidget
{
    Q_OBJECT
public:
    explicit Rectangle(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *) override;

    const QColor &color() const;
    void setColor(const QColor &newColor);

private:
    QColor mColor;
};

#endif // RECTANGLE_H
