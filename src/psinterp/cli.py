# command line interface
import psinterp.interp
import argparse

DESCRIPTION = """
    Perform phase interpolation between PS pixels
"""

def get_cli_args():
    parser = argparse.ArgumentParser(
            prog = "psinterp",
            description = DESCRIPTION,
            )
    parser.add_argument(
            "ifgfile",
            help = "Filename of the interferogram to interpolate"
            )
    parser.add_argument(
            "psfile",
            help = "Filename of PS file",
            )
    parser.add_argument(
            "nrow",
            help = "number of rows of the interferogram",
            )
    parser.add_argument(
            "ncol",
            help = "number of columns of the interferogram",
            )
    parser.add_argument(
            "--outputfile",
            default="",
            help="Filename of the interpolated interferogram",
            )
    parser.add_argument(
            "--N",
            default=20,
            help="Number of neighboring PS pixels used for interpolation",
            )
    parser.add_argument(
            "--rdmax",
            default=101,
            help="Maximum search radius of the PS search [in pixels]",
            )
    parser.add_argument(
            "--alpha",
            default=0.75,
            help="""Parameter controlling the weight of each PS pixel during the
            interpolation: smaller alpha means more weight is assigned to PS
            closer to the center pixel.""",
            )
    return parser.parse_args()

def run_cli():
    args = get_cli_args()
    print("Processing with options:")
    print(vars(args))
    psinterp.interp.interpf(**vars(args))

