#pragma once
#include <Siv3D.hpp>

void NearestNeighbor()
{
	Image image{ U"example/landscape_02.jpeg" };

	auto result = image;
	auto scale = 1.0;

	DynamicTexture texture{ result };

	auto CalculatePos = [&](Vec2 src)
		{
			Point dst;
			dst.x = Floor((src.x + 0.5) / scale);
			dst.y = Floor((src.y + 0.5) / scale);
			return dst;
		};

	auto UpdateImage = [&]()
		{
			Vec2 size = image.size() * scale;
			result = Image{ static_cast<size_t>(size.x), static_cast<size_t>(size.y) };

			for (int w = 0; w < result.width(); w++)
			{
				for (int h = 0; h < result.height(); h++)
				{
					Point pos = CalculatePos({ w,h });
					pos.x = Clamp(pos.x, 0, image.size().x);
					pos.y = Clamp(pos.y, 0, image.size().y);
					result[h][w] = image[pos.y][pos.x];
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
