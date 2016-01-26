// From: www.aya.or.jp/~sanami/peace/
/// ����Ĺ�������饹
/**
 * ������Ǥ�int�򥷥եȤ��ƾ�����ɽ������Τ��Ｑ�餷���Τǡ�
 * �������������եȱ黻���ä��륯�饹���äƤߤ褦���ȡ�
 * ���Ϥ� double �ǡ����Ϥ� int ��
 * ����ɽ���� int ���������ⰷ���롢�ȤΤ��ȡ�
 * int �� 32 �ӥåȤ���ʤ��Ⱥ���Ȼפ���
 * ���եȿ��ϥƥ�ץ졼�Ȱ����ǻ��ꡣ
 * ���� VC �Ǥ��̤��Ǥ���
 */

template <int shifts_, typename Int_ =int>
class GameNum {
private:
	/// ����ѥ��륿����ʥ��ե��Ѵ���
	/**
	 * �¹�®�٤˱ƶ����ʤ���
	 */
	//@{
	template <bool equal_, bool leftLarge_,
			  int leftShifts_, int rightShifts_>
	struct ConvertHelper_;
	template <int leftShifts_, int rightShifts_>
	struct ConvertHelper_<true, false, leftShifts_, rightShifts_> {
		static Int_ run(Int_ v) {
			return v;
		}
	};
	template <int leftShifts_, int rightShifts_>
	struct ConvertHelper_<false, true, leftShifts_, rightShifts_> {
		static Int_ run(Int_ v) {
			return v << leftShifts_ - rightShifts_;
		}
	};
	template <int leftShifts_, int rightShifts_>
	struct ConvertHelper_<false, false, leftShifts_, rightShifts_> {
		static Int_ run(Int_ v) {
			return v >> rightShifts_ - leftShifts_;
		}
	};
	template <int leftShifts_, int rightShifts_>
	Int_ convert_(Int_ v) const {
		return ConvertHelper_
			<(leftShifts_ == rightShifts_), (leftShifts_ > rightShifts_),
			leftShifts_, rightShifts_>::run(v);
	}
	//@}

	static Int_ convDouble(double v) {
		return static_cast<Int_>(v * (1 << shifts_));
	}

public:
	GameNum() : val(0) {}
	explicit GameNum(double v) : val(convDouble(v)) {}
	explicit GameNum(Int_ v) : val(v << shifts_) {}
	template <int shifts2_>
	explicit GameNum(GameNum<shifts2_, Int_> v)
		: val(convert_<shifts_, shifts2_>(v.val)) {}

public:
	operator Int_() const { return getVal(); }
	Int_ getVal() const { return val >> shifts_; }
	double getDouble() const { return (double)val / (1 << shifts_); }
	Int_ getShiftedVal() const { return val; }

public:
	GameNum& operator = (Int_ v) {
		val = v << shifts_;
		return *this;
	}
	template <int shifts2_>
	GameNum& operator = (GameNum<shifts2_, Int_> v) {
		val = convert_<shifts_, shifts2_>(v.val);
		return *this;
	}

	GameNum& operator += (Int_ v) {
		val += v << shifts_;
		return *this;
	}
	template <int shifts2_>
	GameNum& operator += (GameNum<shifts2_, Int_> v) {
		val += convert_<shifts_, shifts2_>(v.val);
		return *this;
	}
	GameNum& operator -= (Int_ v) {
		val -= v << shifts_;
		return *this;
	}
	template <int shifts2_>
	GameNum& operator -= (GameNum<shifts2_, Int_> v) {
		val -= convert_<shifts_, shifts2_>(v.val);
		return *this;
	}
	GameNum& operator *= (Int_ v) {
		val *= v;
		return *this;
	}
	template <int shifts2_>
	GameNum& operator *= (GameNum<shifts2_, Int_> v) {
		val *= v.val;
		val >>= shifts2_;
		return *this;
	}
	GameNum& operator /= (Int_ v) {
		val /= v;
		return *this;
	}
	template <int shifts2_>
	GameNum& operator /= (GameNum<shifts2_, Int_> v) {
		val <<= shifts2_;
		val /= v.val;
		return *this;
	}

	GameNum operator + (Int_ rhs) {
		GameNum ret(*this);
		ret += rhs;
		return ret;
	}
	friend inline GameNum operator + (Int_ lhs, GameNum rhs) {
		return rhs.operator + (lhs);
	}
	template <int shifts2_>
	GameNum operator + (GameNum<shifts2_, Int_> rhs) {
		GameNum ret(*this);
		ret += rhs;
		return ret;
	}
	GameNum operator - (Int_ rhs) {
		GameNum ret(*this);
		ret -= rhs;
		return ret;
	}
	friend inline GameNum operator - (Int_ lhs, GameNum rhs) {
		GameNum ret(lhs);
		ret -= rhs;
		return ret;
	}
	template <int shifts2_>
	GameNum operator - (GameNum<shifts2_, Int_> rhs) {
		GameNum ret(*this);
		ret -= rhs;
		return ret;
	}
	GameNum operator * (Int_ rhs) {
		GameNum ret(*this);
		ret *= rhs;
		return ret;
	}
	friend inline GameNum operator * (Int_ lhs, GameNum rhs) {
		return rhs.operator * (lhs);
	}
	template <int shifts2_>
	GameNum operator * (GameNum<shifts2_, Int_> rhs) {
		GameNum ret(*this);
		ret *= rhs;
		return ret;
	}
	GameNum operator / (Int_ rhs) {
		GameNum ret(*this);
		ret /= rhs;
		return ret;
	}
	friend inline GameNum operator / (Int_ lhs, GameNum rhs) {
		GameNum ret(lhs);
		ret /= rhs;
		return ret;
	}
	template <int shifts2_>
	GameNum operator / (GameNum<shifts2_, Int_> rhs) {
		GameNum ret(*this);
		ret /= rhs;
		return ret;
	}
	bool operator > (Int_ rhs) {
		return operator>(GameNum(rhs));
	}
	template <int shifts2_>
	bool operator > (GameNum<shifts2_, Int_> rhs) {
		return val > convert_<shifts_, shifts2_>(rhs.val);
	}
	bool operator < (Int_ rhs) {
		return operator<(GameNum(rhs));
	}
	template <int shifts2_>
	bool operator < (GameNum<shifts2_, Int_> rhs) {
		return val < convert_<shifts_, shifts2_>(rhs.val);
	}
	bool operator == (Int_ rhs) {
		return operator==(GameNum(rhs));
	}
	template <int shifts2_>
	bool operator == (GameNum<shifts2_, Int_> rhs) {
		return val == convert_<shifts_, shifts2_>(rhs.val);
	}
	bool operator <= (Int_ rhs) {
		return operator<=(GameNum(rhs));
	}
	template <int shifts2_>
	bool operator <= (GameNum<shifts2_, Int_> rhs) {
		return val <= convert_<shifts_, shifts2_>(rhs.val);
	}
	bool operator >= (Int_ rhs) {
		return operator>=(GameNum(rhs));
	}
	template <int shifts2_>
	bool operator >= (GameNum<shifts2_, Int_> rhs) {
		return val >= convert_<shifts_, shifts2_>(rhs.val);
	}

public:
	/// ľ�ܤ����ꤿ���ʤ뤳�Ȥ⤢��Ǥ��礦����
	Int_ val;

}; // end of class GameNum
