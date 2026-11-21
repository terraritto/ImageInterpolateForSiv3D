#pragma once
#include <Siv3D.hpp>

void SincComparison()
{
	constexpr Vec2 REGION_X_VALUE{ -3.0,3.0 };
	constexpr Vec2 REGION_Y_VALUE{ -0.2,1.1 };
	constexpr int n = 1000;
	const auto SceneSize = Scene::Size();

	auto CalcSinc = [](double x)
		{
			return x == 0.0 ? 1.0 : Sin(x) / x;
		};

	auto CalcH = [](double v, double a = -1.0)
		{
			double av = Abs(v);
			double av2 = av * av;
			double av3 = av * av2;
			double r = 0.0;

			if (av <= 1.0)
			{
				r = (a + 2.0) * av3 - (a + 3.0) * av2 + 1.0;
			}
			else if (1.0 < av && av <= 2.0)
			{
				r = a * av3 - 5.0 * a * av2 + 8.0 * a * av - 4.0 * a;
			}

			return r;
		};

	double a = -1.0;

	Array<double> sinc(n+1, 0.0), h(n+1, 0.0);
	for (int i = 0; i <= n; i++)
	{
		double v = Math::Lerp(REGION_X_VALUE.x, REGION_X_VALUE.y, i / static_cast<double>(n));
		sinc[i] = CalcSinc(v * Math::Pi);
		h[i] = CalcH(v, a);
	}

	auto DrawFunc = [&](const Array<double>& v, const ColorF& c)
		{
			for (int i = 0; i < v.size() - 1; i++)
			{
				Vec2 s1, s2;
				s1.x = i / static_cast<double>(n) * SceneSize.x;
				s2.x = (i+1) / static_cast<double>(n) * SceneSize.x;
				s1.y = (1.0 - (v[i] - REGION_Y_VALUE.x) / (REGION_Y_VALUE.y - REGION_Y_VALUE.x)) * SceneSize.y;
				s2.y = (1.0 - (v[i] - REGION_Y_VALUE.x) / (REGION_Y_VALUE.y - REGION_Y_VALUE.x)) * SceneSize.y;

				Line{ s1,s2 }.draw(c);
			}
		};

	while (System::Update())
	{
		ClearPrint(); Print << a;

		DrawFunc(sinc, Palette::Green); DrawFunc(h, Palette::Red);

		bool isKeyDown = false;
		if (Key1.down())
		{
			isKeyDown = true; a -= 0.1;
		}
		else if (Key2.down())
		{
			isKeyDown = true; a += 0.1;
		}

		if (isKeyDown)
		{
			for (int i = 0; i <= n; i++)
			{
				double v = Math::Lerp(REGION_X_VALUE.x, REGION_X_VALUE.y, i / static_cast<double>(n));
				h[i] = CalcH(v, a);
			}
		}
	}
}
