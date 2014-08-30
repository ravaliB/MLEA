#include <vector>
#include <string>

// Convert lines with ROC coordinates to normal deviate scale.
// Ignore lines beginning with "#" (9/26/94)
double ppndf (double p);

void LoadScoreFile(std::vector<double> &res, const std::string &filename);

void
Compute_ROC (std::vector<double> &true_scores, std::vector<double> &false_scores,
	     std::vector<std::pair<double, double> > &Pdet,
	     std::vector<double> &DCF_Dec,
	     std::vector<double> &DCF_best);
