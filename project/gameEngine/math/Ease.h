#pragma once

#include <cmath>
#include <numbers>

/// <summary>
/// イージング
/// </summary>
class Ease 
{
public:

   static constexpr float pi = std::numbers::pi_v<float>;

   /// <summary>
   /// 線形補間
   /// </summary>
   /// <param name="t">0.0〜1.0の補間値</param>
   /// <returns>補間後の値</returns>
   static float Linear(float t) 
   {
       return t;
   }

   /// <summary>
   /// イーズイン(加速)
   /// </summary>
   /// <param name="t">0.0〜1.0の補間値</param>
   /// <returns>補間後の値</returns>
   static float InQuad(float t)
   {
       return t * t;
   }

   /// <summary>
   /// イーズイン(加速)
   /// </summary>
   /// <param name="t">0.0〜1.0の補間値</param>
   /// <returns>補間後の値</returns>
   static float InCubic(float t) 
   {
       return t * t * t;
   }

   /// <summary>
   /// イーズイン(加速)
   /// </summary>
   /// <param name="t">0.0〜1.0の補間値</param>
   /// <returns>補間後の値</returns>
   static float InQuart(float t) 
   {
       return t * t * t * t;
   }

   /// <summary>
   /// イーズイン(加速)
   /// </summary>
   /// <param name="t">0.0〜1.0の補間値</param>
   /// <returns>補間後の値</returns>
   static float InQuint(float t) 
   {
       return t * t * t * t * t;
   }

   /// <summary>
   /// イーズアウト(減速)
   /// </summary>
   /// <param name="t">0.0〜1.0の補間値</param>
   /// <returns>補間後の値</returns>
   static float OutQuad(float t)
   {
       return 1.0f - (1.0f - t) * (1.0f - t);
   }

   /// <summary>
   /// イーズアウト(減速)
   /// </summary>
   /// <param name="t">0.0〜1.0の補間値</param>
   /// <returns>補間後の値</returns>
   static float OutCubic(float t)
   {
       return 1.0f - std::pow(1.0f - t, 3.0f);
   }

   /// <summary>
   /// イーズアウト(減速)
   /// </summary>
   /// <param name="t">0.0〜1.0の補間値</param>
   /// <returns>補間後の値</returns>
   static float OutQuart(float t)
   {
       return 1.0f - std::pow(1.0f - t, 4.0f);
   }

   /// <summary>
   /// イーズアウト(減速)
   /// </summary>
   /// <param name="t">0.0〜1.0の補間値</param>
   /// <returns>補間後の値</returns>
   static float OutQuint(float t) 
   {
       return 1.0f - std::pow(1.0f - t, 5.0f);
   }

   /// <summary>
   /// イーズインアウト(加速→減速)
   /// </summary>
   /// <param name="t">0.0〜1.0の補間値</param>
   /// <returns>補間後の値</returns>
   static float InOutQuad(float t)
   {
       return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
   }

   /// <summary>
   /// イーズインアウト(加速→減速)
   /// </summary>
   /// <param name="t">0.0〜1.0の補間値</param>
   /// <returns>補間後の値</returns>
   static float InOutCubic(float t)
   {
       return t < 0.5f ? 4.0f * t * t * t : 1.0f - static_cast<float>(std::pow(-2.0f * t + 2.0f, 3.0f)) / 2.0f;
   }

   /// <summary>
   /// イーズインアウト(加速→減速)
   /// </summary>
   /// <param name="t">0.0〜1.0の補間値</param>
   /// <returns>補間後の値</returns>
   static float InOutQuart(float t) 
   {
       return t < 0.5f ? 8.0f * static_cast<float>(std::pow(t, 4.0f)) : 1.0f - static_cast<float>(std::pow(-2.0f * t + 2.0f, 4.0f)) / 2.0f;
   }

   /// <summary>
   /// イーズインアウト(加速→減速)
   /// </summary>
   /// <param name="t">0.0〜1.0の補間値</param>
   /// <returns>補間後の値</returns>
   static float InOutQuint(float t)
   {
       return t < 0.5f ? 16.0f * static_cast<float>(std::pow(t, 5.0f)) : 1.0f - static_cast<float>(std::pow(-2.0f * t + 2.0f, 5.0f)) / 2.0f;
   }
   
   /// <summary>
   /// バウンス
   /// </summary>
   /// <param name="t">0.0〜1.0の補間値</param>
   /// <returns>補間後の値</returns>
   static float OutBounce(float t) 
   {
       const float n1 = 7.5625f;
       const float d1 = 2.75f;

       if (t < 1 / d1)
       {
           return n1 * t * t;
       }
       else if (t < 2 / d1) 
       {
           t -= 1.5f / d1;
           return n1 * t * t + 0.75f;
       }
       else if (t < 2.5f / d1) 
       {
           t -= 2.25f / d1;
           return n1 * t * t + 0.9375f;
       }
       else
       {
           t -= 2.625f / d1;
           return n1 * t * t + 0.984375f;
       }
   }

   /// <summary>
   /// バックアウト(バウンド的に出現)
   /// </summary>
   /// <param name="t">0.0〜1.0の補間値</param>
   /// <returns>補間後の値</returns>
   static float OutBack(float t) 
   {
       const float c1 = 1.70158f;
       const float c3 = c1 + 1.0f;

       float t1 = t - 1.0f;
       return 1.0f + c3 * static_cast<float>(std::pow(t1, 3.0f)) + c1 * static_cast<float>(std::pow(t1, 2.0f));
   }

   /// <summary>
   /// イーズインサイン
   /// </summary>
   /// <param name="t">0.0〜1.0の補間値</param>
   /// <returns>補間後の値</returns>
   static float InSine(float t)
   {
       return 1.0f - std::cos((t * pi) / 2.0f);
   }

   /// <summary>
   /// イーズアウトサイン
   /// </summary>
   /// <param name="t">0.0〜1.0の補間値</param>
   /// <returns>補間後の値</returns>
   static float OutSine(float t)
   {
       return std::sin((t * pi) / 2.0f);
   }

   /// <summary>
   /// イーズインアウトサイン
   /// </summary>
   /// <param name="t">0.0〜1.0の補間値</param>
   /// <returns>補間後の値</returns>
   static float InOutSine(float t)
   {
       return -(std::cos(pi * t) - 1.0f) / 2.0f;
   }

};
