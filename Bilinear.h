#pragma once
#include <Siv3D.hpp>

void Bilinear()
{
	Image image{ U"example/landscape_02.jpeg" };

	auto result = image;
	auto scale = 1.0;

	DynamicTexture texture{ result };

	auto UpdateImage = [&]()
		{
			Vec2 size = image.size() * scale;
			result = Image{ static_cast<size_t>(size.x), static_cast<size_t>(size.y) };

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

					// 閾値を計算
					auto [alpha, beta] = (pos - minPos) / (maxPos - minPos);
					if (isnan(alpha)) { alpha = 0.0; }
					if (isnan(beta)) { beta = 0.0; }

					// Qを用意
					ColorF Q1 = image[minPos.y][minPos.x];
					ColorF Q2 = image[minPos.y][maxPos.x];
					ColorF Q3 = image[maxPos.y][minPos.x];
					ColorF Q4 = image[maxPos.y][maxPos.x];

					// 線形補間
					auto Q_y1 = Q1.lerp(Q2, alpha);
					auto Q_y2 = Q3.lerp(Q4, alpha);
					auto Q = Q_y1.lerp(Q_y2, beta);

					// 格納
					result[h][w] = Q;
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
