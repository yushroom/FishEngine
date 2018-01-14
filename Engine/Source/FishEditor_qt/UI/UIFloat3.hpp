#ifndef UIFLOAT3_HPP
#define UIFLOAT3_HPP

#include <QWidget>

#include <string>

namespace Ui {
class UIFloat3;
}

class UIFloat3 : public QWidget
{
	Q_OBJECT

public:
	explicit UIFloat3(std::string const & label, float x, float y, float z, QWidget *parent = 0);
	~UIFloat3();

//    inline float x() const { return m_x; }
//    inline float y() const { return m_y; }
//    inline float z() const { return m_z; }

	// return true if the value is modified by Inspector
	bool CheckUpdate(std::string const & label, float & x, float & y, float & z);

private Q_SLOTS:
	inline void setX(float v) { m_x = v; m_changed = true; }
	inline void setY(float v) { m_y = v; m_changed = true; }
	inline void setZ(float v) { m_z = v; m_changed = true; }

private:
	Ui::UIFloat3 *ui;

	bool        m_changed = false;

	std::string m_label;
	float       m_x;
	float       m_y;
	float       m_z;

};

#endif // UIFLOAT3_HPP
