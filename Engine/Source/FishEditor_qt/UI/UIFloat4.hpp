#ifndef UIFLOAT4_HPP
#define UIFLOAT4_HPP

#include <QWidget>

#include <string>

namespace Ui {
class UIFloat4;
}

class UIFloat4 : public QWidget
{
	Q_OBJECT

public:
	explicit UIFloat4(std::string const & label, float x, float y, float z, float w, QWidget *parent = 0);
	~UIFloat4();

	bool CheckUpdate(std::string const & label, float & x, float & y, float & z, float & w);

private Q_SLOTS:
	inline void setX(float v) { m_x = v; m_changed = true; }
	inline void setY(float v) { m_y = v; m_changed = true; }
	inline void setZ(float v) { m_z = v; m_changed = true; }
	inline void setW(float v) { m_w = v; m_changed = true; }

private:
	Ui::UIFloat4 *ui;

	bool        m_changed = false;

	std::string m_label;
	float       m_x;
	float       m_y;
	float       m_z;
	float       m_w;
};

#endif // UIFLOAT4_HPP
