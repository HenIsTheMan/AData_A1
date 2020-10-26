#include "Frac.h"

bool Frac::form = 0;

const Frac& Frac::Simplify() noexcept{
	if(num && denom){
		std::function<const int(const int&, const int&)> HCF = [&HCF](const int& a, const int& b){
			const int& smaller = a < b ? a : b;
			const int&& remainder = (a > b ? a : b) % smaller;
			if(!remainder){
				return smaller;
			}
			return HCF(smaller, remainder);
		};
		int&& divisor = HCF(abs(num), abs(denom));
		this->Set(num / divisor * (denom < 0 ? -1 : 1), abs(denom) / divisor);
	}
	return *this;
}

const int Compare(const Frac& left, const Frac& right) noexcept{
	int &&LHS = left.num * right.denom, &&RHS = right.num * left.denom;
	return LHS == RHS ? 0 : (LHS > RHS ? 1 : -1);
}

const bool operator>>(const std::istream&, Frac& frac){
	char inputArr[50];
	fgets(inputArr, 50, stdin);
	str input(inputArr);
	while(input[0] == ' '){
		input.erase(input.begin());
	}
	while(input.length() && (input[input.length() - 1] == ' ' || input[input.length() - 1] == '\n')){
		input.pop_back();
	}
	try{
		size_t spacePos = input.find(' '), slashPos = input.find('/');
		short index = 0;
		while((int(input[index]) > 47 && int(input[index]) < 58) || int(input[index]) == 45){
			++index;
		}
		if(slashPos == std::string::npos || (input[index] != ' ' && input[index] != '/') || ((int(input[0]) < 48 || int(input[0]) > 57) && int(input[0] != 45))){
			throw std::runtime_error("");
		}
		const int&& denom = stoi(input.substr(slashPos + 1));
		if(denom == 0){
			throw("\nError: denominator of input fraction is 0!\n");
		}
		if(std::count(input.begin(), input.end(), ' ') && stoi(input.substr(spacePos + 1, slashPos)) == denom){
			throw std::runtime_error("");
		}
		if(std::count(input.begin(), input.end(), ' ')){
			const int&& whole = stoi(input.substr(0, spacePos));
			int num = stoi(input.substr(spacePos + 1, slashPos));
			const short&& amtOfNeg = short(whole < 0) + short(num < 0) + short(denom < 0);
			num = abs(whole) * abs(denom) + abs(num);
			frac.Set(num * (amtOfNeg & 1 ? -1 : 1), abs(denom));
		} else{
			const int&& num = stoi(input.substr(0, slashPos));
			frac.Set(num, denom);
		}
		frac.Simplify();
	} catch(const char* const errMsg){
		printf(errMsg);
		return 1;
	} catch(const std::runtime_error&){
		printf("\nRuntime error: Invalid fraction input!\n");
		return 1;
	}
	return 0;
}

std::ostream& operator<<(std::ostream& os, const Frac& frac) noexcept{
	if(!frac.denom){
		return os << "Undefined";
	}
	const int&& remainder = frac.num % frac.denom;
	if(!remainder){
		return os << frac.num / frac.denom;
	}
	if(abs(frac.num) < abs(frac.denom) || !Frac::form){
		return os << frac.num << '/' << frac.denom;
	}
	return os << int(frac.num / frac.denom) << ' ' << abs(remainder) << '/' << frac.denom;
}

Frac::Frac(const int& num, const int& denom) noexcept{
	this->num = num;
	this->denom = denom;
}

const Frac Frac::operator+(const Frac& RHS) const noexcept{
	return Frac(num * RHS.denom + RHS.num * denom, denom * RHS.denom).Simplify();
}

const Frac Frac::operator-(const Frac& RHS) const noexcept{
	return Frac(num * RHS.denom - RHS.num * denom, denom * RHS.denom).Simplify();
}

const Frac Frac::operator*(const Frac& RHS) const noexcept{
	return Frac(num * RHS.num, denom * RHS.denom).Simplify();
}

const Frac Frac::operator/(const Frac& RHS) const noexcept{
	return Frac(num * RHS.denom, denom * RHS.num).Simplify();
}

const bool& Frac::ToggleForm() noexcept{
	form = !form;
	return form;
}

const int Frac::GetNum() const noexcept{
	return num;
}

const int Frac::GetDenom() const noexcept{
	return denom;
}

void Frac::Set(const int& num, const int& denom) noexcept{
	this->num = num;
	this->denom = denom;
}

void Frac::SetNum(const int& num) noexcept{
	this->num = num;
}

void Frac::SetDenom(const int& denom) noexcept{
	this->denom = denom;
}