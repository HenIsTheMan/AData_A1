#pragma once
#include "Src.h"

class Frac final{
	int num, denom;
	static bool form;

	const Frac& Simplify() noexcept;

	friend const int Compare(const Frac&, const Frac&) noexcept;
	friend const bool operator>>(const std::istream&, Frac&);
	friend std::ostream& operator<<(std::ostream&, const Frac&) noexcept;
public:
	Frac() = default;
	Frac(const int&, const int&) noexcept;
	const Frac operator+(const Frac&) const noexcept;
	const Frac operator-(const Frac&) const noexcept;
	const Frac operator*(const Frac&) const noexcept;
	const Frac operator/(const Frac&) const noexcept;
	static const bool& ToggleForm() noexcept;

	///Getters
	const int GetNum() const noexcept;
	const int GetDenom() const noexcept;

	///Setters
	void Set(const int&, const int&) noexcept;
	void SetNum(const int&) noexcept;
	void SetDenom(const int&) noexcept;
};