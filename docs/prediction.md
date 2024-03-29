# Prediction

This is the predictor part.

| Method |            Meaning            | file | Average Success Rate |
| :----: | :----------------------------:|:-: | :------------------: |
|   A    |        **A**lways taken       | `predictor-static.hpp` |        57.13%        |
|   ~A   |    **A**lways **n**ot taken   | ~ |        42.87%        |
|   S    |     **S**aturating counter    | `predictor-sature.hpp` |        82.52%        |
|   T8   | **T**wo-level Adaptive (8-bit)| `predictor.hpp` |        84.17%        |

The test result is listed below. Though the number is not much pretty in the Average Success Rate, caused by `pi`, the Two-level Adaptive training prediction method shows better results.

This is the detailed datasheet.

| Test Name      | Hit: A   | Hit: S   | Hit: T8  | Total    | A%     | S%     | T8%    |
| -------------- | -------- | -------- | -------- | -------- | ------ | ------ | ------ |
| sample         | 0        | 0        | 0        | 0        | /      | /      | /      |
| array_test1    | 10       | 12       | 12       | 22       | 45.45% | 54.55% | 54.55% |
| array_test2    | 13       | 14       | 15       | 26       | 50%    | 53.85% | 57.69% |
| basicopt1      | 63642    | 127835   | 154145   | 155139   | 41.02% | 82.40% | 99.36% |
| gcd            | 45       | 80       | 85       | 120      | 37.50% | 66.67% | 70.83% |
| lvalue2        | 2        | 2        | 2        | 6        | 33.33% | 33.33% | 33.33% |
| manyarguments  | 2        | 2        | 2        | 10       | 20%    | 20%    | 20%    |
| naive          | 0        | 0        | 0        | 0        | /      | /      | /      |
| qsort          | 140018   | 174888   | 192210   | 200045   | 69.99% | 87.42% | 96.08% |
| bulgarian      | 35289    | 67508    | 67688    | 71493    | 49.36% | 94.43% | 94.68% |
| expr           | 69       | 93       | 84       | 111      | 62.16% | 83.78% | 75.68% |
| magic          | 31801    | 53215    | 59254    | 67869    | 46.86% | 78.41% | 87.31% |
| multiarray     | 132      | 138      | 138      | 162      | 81.48% | 85.19% | 85.19% |
| statement_test | 82       | 114      | 116      | 202      | 40.59% | 56.44% | 57.43% |
| tak            | 15160    | 44754    | 50656    | 60639    | 25.00% | 73.80% | 83.54% |
| superloop      | 55272    | 408131   | 431615   | 435027   | 12.71% | 93.82% | 99.22% |
| pi             | 23066734 | 32925339 | 33508520 | 39956380 | 57.73% | 82.40% | 83.86% |
| heart          | 110      | 110      | 121      | 154      | 71.43% | 71.43% | 78.57% |
| hanoi          | 8732     | 10664    | 17305    | 17457    | 50.02% | 61.09% | 99.13% |
| queens         | 28362    | 56576    | 63266    | 77116    | 36.78% | 73.36% | 82.04% |