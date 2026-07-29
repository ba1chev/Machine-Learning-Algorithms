#include <err.h>
#include <cstring>
#include <exception>

#include "source/utils.hpp"
#include "source/learning_models/learning_model_type.h"
#include "source/build/execution_builder/execution_builder.h"

int main(int argc, const char* argv[]) {
    if (argc < 2 || !argv[1]) {
        errx(1, "Usage: main <train|predict> ...");
    }

    ExecutionBuilder builder;

    try {
        if (!strcmp(argv[1], "train")) {
            // main train <model_type> <train.csv> [hyperparams...] [scaler] <model_out>
            if (argc < 5) {
                errx(1, "Usage: main train <model_type> <train.csv> [hyperparams...] [scaler] <model_out>");
            }

            LearningModelType type = INPUT::parse_type(argv[2]);
            const char* model_out = argv[argc - 1];

            // Optional optimizer and scaler tokens sit just before <model_out>,
            // recognised by value (order: [optimizer] [scaler]). Either may be absent.
            std::string scaler_type = "none";
            std::string optimizer_type = "gradient_descent";
            int token_index = argc - 2;
            int recognised_tokens = 0;
            while (token_index > 2 && recognised_tokens < 2) {
                const char* token = argv[token_index];
                if (INPUT::is_scaler_token(token)) {
                    scaler_type = token;
                } else if (INPUT::is_optimizer_token(token)) {
                    optimizer_type = token;
                } else {
                    break;
                }
                token_index -= 1;
                recognised_tokens += 1;
            }

            int validator_argc = argc - 3 - recognised_tokens;

            builder.train(type, validator_argc, argv + 2, optimizer_type, scaler_type, model_out);
        } else if (!strcmp(argv[1], "predict")) {
            // main predict <model_in> <input.csv> <output.csv>
            if (argc != 5) {
                errx(1, "Usage: main predict <model_in> <input.csv> <output.csv>");
            }

            builder.predict(argv[2], argv[3], argv[4]);
        } else {
            errx(1, "Unknown mode '%s' (expected 'train' or 'predict')", argv[1]);
        }
    } catch (const std::exception& e) {
        errx(1, "%s", e.what());
    }

    return 0;
}
