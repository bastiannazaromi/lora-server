<div class="content-wrapper">
	<!-- Content Header (Page header) -->
	<div class="content-header">
		<div class="container-fluid">
			<div class="row mb-2">
				<div class="col-sm-6">
					<h1 class="m-0"><?= $title; ?></h1>
				</div><!-- /.col -->
				<div class="col-sm-6">
					<ol class="breadcrumb float-sm-right">
						<li class="breadcrumb-item"><a href="<?= base_url('pelanggan'); ?>">Data Pelanggan</a></li>
						<li class="breadcrumb-item active">Edit Pelanggan</li>
					</ol>
				</div><!-- /.col -->
			</div><!-- /.row -->
		</div><!-- /.container-fluid -->
	</div>
	<!-- /.content-header -->

	<!-- Main content -->
	<section class="content">
		<div class="container-fluid">
			<div class="row">
				<div class="col-xl-12">
					<div class="card card-secondary">
						<div class="card-header">
							<h3 class="card-title">Form Edit Pelanggan</h3>
						</div>
						<div class="card-body">
							<form action="<?= base_url('pelanggan/update'); ?>" method="post">
								<input type="hidden" name="id" value="<?php echo $pelanggan->id; ?>">
								<div class="form-group">
									<label>Serial Number</label>
									<input type="text" name="serialNumber" class="form-control" value="<?php echo $pelanggan->serialNumber; ?>">
									<span class="text-danger"><?php echo form_error('serialNumber'); ?></span>
								</div>
								<div class="form-group">
									<label>Nama</label>
									<input type="text" name="nama" class="form-control" value="<?php echo $pelanggan->nama; ?>">
									<span class="text-danger"><?php echo form_error('nama'); ?></span>
								</div>
								<button type="submit" class="btn btn-success">Simpan</button>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div><!-- /.container-fluid -->
	</section>
	<!-- /.content -->
</div>