#include "..\Includes.h"

namespace COM
{
	CapResult CapAnalyzer(CapCondition capCondition)
	{
		CapResult capResult;
		if (2 > capCondition.maxCount || 2 > capCondition.countInGroup ||
			1 > capCondition.groupCount || capCondition.USL == capCondition.LSL)
			return capResult;

		capCondition.countInGroup = capCondition.maxCount / capCondition.groupCount;
		capResult.min = capResult.max = capCondition.val[0];

		double buffSum = 0.;
		double unitSum = 0.;
		double unitAvg = 0.;

		List<BOOL> isSelected;
		for (int nCnt = 0; nCnt < capCondition.maxCount; nCnt++)
		{
			if (capResult.min > capCondition.val[nCnt])
				capResult.min = capCondition.val[nCnt];

			if (capResult.max < capCondition.val[nCnt])
				capResult.max = capCondition.val[nCnt];

			if (capCondition.USL < capCondition.val[nCnt] || capCondition.LSL > capCondition.val[nCnt])
			{
				isSelected.Add(FALSE);
				continue;
			}
			capResult.validCount++;
			isSelected.Add(TRUE);
			buffSum += capCondition.val[nCnt];
		}

		if (2 > capResult.validCount)
			return capResult;

		capResult.avg = buffSum / double(capResult.validCount);
		capResult.gap = capResult.max - capResult.min;
		capResult.yield = double(capResult.validCount) / double(capCondition.maxCount);

		buffSum = 0.;
		int	validCountInGroupSum = 0;
		for (int i = 0; i < capCondition.groupCount; i++)
		{
			double subgroupVariance = 0.;
			unitSum = 0.;
			int	validCountInGroup = 0;
			for (int j = 0; j < capCondition.countInGroup; j++)
			{
				if (capCondition.maxCount <= (i * capCondition.countInGroup + j))
					break;

				if (isSelected[i * capCondition.countInGroup + j])
				{
					validCountInGroup++;
					unitSum += capCondition.val[i * capCondition.countInGroup + j];
				}
			}

			if (2 > validCountInGroup)
				return capResult;

			validCountInGroupSum += validCountInGroup;
			unitAvg = unitSum / double(validCountInGroup);

			for (int j = 0; j < capCondition.countInGroup; j++)
			{
				if (capCondition.maxCount <= (i * capCondition.countInGroup + j))
					break;

				if (isSelected[i * capCondition.countInGroup + j])
				{
					double deviation = capCondition.val[i * capCondition.countInGroup + j] - unitAvg;
					subgroupVariance += pow(deviation, 2);
				}
			}

			subgroupVariance /= (validCountInGroup - 1); // 자유도 적용
			buffSum += subgroupVariance; // 부분군별 분산 합산
		}

		if (2 > validCountInGroupSum)
			return capResult;

		capResult.ST = sqrt(buffSum / capCondition.groupCount);

		if (0 == capResult.ST)
			return capResult;

		buffSum = 0.;
		unitSum = 0.;
		for (int i = 0; i < capCondition.maxCount; i++)
		{
			if (!isSelected[capCondition.maxCount - i - 1])
				continue;

			unitSum += capCondition.val[capCondition.maxCount - i - 1];
		}

		unitAvg = unitSum / capResult.validCount;
		for (int i = 0; i < capCondition.maxCount; i++)
		{
			if (!isSelected[capCondition.maxCount - i - 1])
				continue;

			buffSum += pow((capCondition.val[capCondition.maxCount - i - 1] - unitAvg), 2);
		}

		capResult.LT = sqrt(buffSum / (double(capResult.validCount) - 1.));

		if (0 == capResult.LT)
			return capResult;

		capResult.K = (((capCondition.USL + capCondition.LSL) / 2.) - capResult.avg) / ((capCondition.USL - capCondition.LSL) / 2.);
		capResult.CP = (capCondition.USL - capCondition.LSL) / (6. * capResult.ST);
		capResult.PP = (capCondition.USL - capCondition.LSL) / (6. * capResult.LT);

		capResult.CPU = (capCondition.USL - capResult.avg) / (3 * capResult.ST);
		capResult.CPL = (capResult.avg - capCondition.LSL) / (3 * capResult.ST);

		capResult.PPU = (capCondition.USL - capResult.avg) / (3 * capResult.LT);
		capResult.PPL = (capResult.avg - capCondition.LSL) / (3 * capResult.LT);

		if (CapCondition::Linear == capCondition.distributionType)
		{
			capResult.CPK = max(0., min(capResult.CPU, capResult.CPL));
			capResult.PPK = max(0., min(capResult.PPU, capResult.PPL));
		}
		else
		{
			capResult.CPK = max(0., (1 - capResult.K) * capResult.CP);
			capResult.PPK = max(0., (1 - capResult.K) * capResult.PP);
		}
		capResult.comp = (TRUE);
		return capResult;
	}
}

