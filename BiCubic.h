#pragma once
#include <Siv3D.hpp>

void BiCubic()
{
	Image image{ U"example/landscape_02.jpeg" };

	auto result = image;
	auto scale = 1.0;

	DynamicTexture texture{ result };

	auto UpdateImage = [&]()
		{
			Vec2 size = image.size() * scale;
			result = Image{ static_cast<size_t>(size.x), static_cast<size_t>(size.y) };

			auto CalcH = [](double v, double a=-1.0)
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

			auto ConvertIndex = [](int v) {return v + 1; };

			for (int w = 0; w < result.width(); w++)
			{
				for (int h = 0; h < result.height(); h++)
				{
					// 点の位置を計算
					Vec2 pos = Vec2{ w,h } / scale;

					// 矩形の最小最大を計算
					Point minPos, maxPos;
					minPos.x = Floor(pos.x); minPos.y = Floor(pos.y);
					maxPos.x = Ceil(pos.x); maxPos.y = Ceil(pos.y);

					// 画像内に限定
					minPos.x = Clamp(minPos.x, 0, image.size().x);
					minPos.y = Clamp(minPos.y, 0, image.size().y);
					maxPos.x = Clamp(maxPos.x, 0, image.size().x);
					maxPos.y = Clamp(maxPos.y, 0, image.size().y);

					// 4x4行列
					Array<Array<Vec3>> m(4, Array<Vec3>(4, Vec3::Zero()));
					for (int i = -1; i <= 2; i++)
					{
						for (int j = -1; j <= 2; j++)
						{
							// Look-up
							auto p = Point{ i, j } + minPos;
							p.x = Clamp(p.x, 0, image.size().x - 1);
							p.y = Clamp(p.y, 0, image.size().y - 1);
							ColorF v = image[p.y][p.x];

							// 値を保持
							m[ConvertIndex(j)][ConvertIndex(i)] = v.rgb();
						}
					}

					// 距離を計算
					Array<double> hx(4, 0.0), hy(4, 0.0);
					for (int i = -1; i <= 2; i++)
					{
						Point p = Point{ i, i } + minPos;
						Vec2 d = pos - p;

						hx[ConvertIndex(i)] = CalcH(Abs(d.x));
						hy[ConvertIndex(i)] = CalcH(Abs(d.y));
					}

					// 行列計算
					Array<Vec3> t(4, Vec3::Zero());
					for (int j = 0; j < 4; j++)
					{
						for (int i = 0; i < 4; i++)
						{
							Vec3 c = m[j][i];
							t[j] += c * hx[i];
						}
					}
					Vec3 r = Vec3::Zero();
					for (int i = 0; i < 4; i++)
					{
						r += hy[i] * t[i];
					}

					// 格納
					result[h][w] = ColorF{ r, 1.0 };
				}
			}

			texture = DynamicTexture{ result };
		};

	while (System::Update())
	{
		texture.draw();

		bool isKeyDown = false;
		if (Key1.down())
		{
			scale /= 2;
			isKeyDown = true;
		}
		else if (Key2.down())
		{
			scale *= 2;
			isKeyDown = true;
		}

		if (isKeyDown)
		{
			UpdateImage();
		}
	}
}
